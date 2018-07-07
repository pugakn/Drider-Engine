#include "dr_graphics_application.h"

#include <functional>
#include <random>

#include <dr_aabb_collider.h>
#include <dr_animation.h>
#include <dr_animator_component.h>
#include <dr_bone_attach_object.h>
#include <dr_camera.h>
#include <dr_camera_component.h>
#include <dr_camera_manager.h> 
#include <dr_context_manager.h>
#include <dr_degree.h>
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_gameObject.h>
#include <dr_graph.h> //Scene graph
#include <dr_graphics_api.h> 
#include <dr_graphics_driver.h>
#include <dr_index_buffer.h>
#include <dr_input_manager.h>
#include <dr_joystick.h>
#include <dr_keyboard.h>
#include <dr_model.h>
#include <dr_mouse.h>
#include <dr_logger.h>
#include <dr_radian.h>
#include <../../Utils/Include/dr_random.h>
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_scoped_timer.h>
#include <dr_script_component.h>
#include <dr_script_core.h>
#include <dr_script_engine.h>
#include <dr_skeleton.h>
#include <dr_swap_chain.h>
#include <dr_vertex_buffer.h>
#include <dr_vector3d.h>
#include <dr_time.h>

#include "AABBDebug.h"
#include "AnimationTechnique.h"
#include "FrustumDebug.h"
#include "LinesTechnique.h"
#include "SkeletonDebug.h"
#include "SpiderAI.h"
#include "SpiderBehavior.h"
#include "SpiderPlayer.h"
#include "StaticMeshTechnique.h"


namespace driderSDK {

GraphicsApplication::GraphicsApplication() 
  : m_drawMeshes(true), 
    m_lockView(true)
{}

GraphicsApplication::~GraphicsApplication() {}

void
GraphicsApplication::postInit() {
  
  IDClass<int>::ID();

  initModules();   
  initInputCallbacks();
  loadResources();
  initScriptEngine();
  createTechniques();
  createScene();
  
  SceneGraph::start();

  Time::update();
    
  m_light[0].m_vec4Position = {100, 100, 100, 2000};
  m_light[0].m_vec4Color = {255, 0, 0, 1};

  /*for (Int32 i = 0; i < 32; ++i) {

  }*/
; 
  m_renderMan.lights = &m_light;

  //m_renderMan.init();

  m_timer.init();
}

void 
GraphicsApplication::postUpdate() {
  Time::update();
  InputManager::update();
  

  if (m_right && m_center) {
    
    /*m_right->getTransform().rotate({0, Math::PI * Time::getDelta(), 0});
    m_center->getTransform().rotate({0, Math::PI * Time::getDelta(), 0});*/
  }

  //ScopedTimer{},

  playerMovement();

  if (!m_lockView) {
    playerRotation();
  }

  SceneGraph::update();
}

void 
GraphicsApplication::postRender() {

  GraphicsDriver::API().clear();

  /*m_renderMan.draw(GraphicsDriver::API().getBackBufferRT(),
                   GraphicsDriver::API().getDepthStencil());*/

  Int32 queryFlags = QUERY_PROPERTY::kAny;

  auto& camera = *CameraManager::getActiveCamera();

  m_animTech->setCamera(&camera);
  m_staticTech->setCamera(&camera);
  m_linesTech->setCamera(&camera);

  auto mainC = CameraManager::getCamera(m_camNames[0]);
  
  auto& dc = GraphicsAPI::getDeviceContext();
  
  if (m_drawMeshes) {

    auto points = calculatePoints();

    for (SizeT i = 0; i < m_paths.size(); ++i) {

      m_paths[i].pushPoint(points[i]);

      m_pathRenders[i].draw(m_linesTech.get());

      m_paths[i].popPoint();

    }    

    std::vector<QueryObjectInfo> queryRes;

    //ScopedTimer{},
    queryRes = SceneGraph::query(*mainC,  
                                  QUERY_ORDER::kBackToFront, 
                                  queryFlags);

    dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

    for (auto& queryObj : queryRes) {
    
      Technique* current;

      if (queryObj.bones) {
        current = m_animTech.get();
        dynamic_cast<AnimationTechnique*>(current)->setBones(*queryObj.bones);
      }
      else {
        current = m_staticTech.get();
      }

      current->setWorld(&queryObj.world);

      auto material = queryObj.mesh.material.lock();

      if (material) {
        material->set();
      }

      if (current->prepareForDraw()) {
        queryObj.mesh.indexBuffer->set(dc);
        queryObj.mesh.vertexBuffer->set(dc);

        dc.draw(queryObj.mesh.indicesCount, 0, 0);
      }

    }
  }

  GraphicsDriver::API().swapBuffers();
}

void 
GraphicsApplication::postDestroy() {
  destroyModules();
}

void GraphicsApplication::recompileShaders() {
  
  if (m_animTech) {
    m_animTech->destroy();
  }

  if (m_staticTech) {
    m_staticTech->destroy();
  }
  
  if (m_linesTech) {
    m_linesTech->destroy();
  }

  createTechniques();
}

void 
GraphicsApplication::initModules() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height, 
                          m_hwnd);
  Time::startUp();
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  ResourceManager::startUp();
  CameraManager::startUp();
  ScriptEngine::startUp();
  ContextManager::startUp();
  SceneGraph::startUp();

  //m_renderMan.init();
}

void 
GraphicsApplication::initInputCallbacks() {
  
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kP,
                        std::bind(&RenderMan::recompile, &m_renderMan)); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k9,
                        &SceneGraph::buildOctree); 

  auto spawnSp =
  [this]() {
    for (Int32 i = 0; i < 100; ++i) {
      auto obj = m_spiderSpawn.spawn();
      auto ai = obj->getComponent<SpiderAI>();
      ai->setPath(&m_paths[Random::get(0, (Int32)m_paths.size() - 1)]);
    }
  };

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kH,
                        spawnSp); 

  auto pushPoint = 
  [this]() {

    auto points = calculatePoints();

    for (SizeT i = 0; i < m_paths.size(); ++i) {
      m_paths[i].pushPoint(points[i]);
    }
  };

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kSPACE, 
                        pushPoint);

  /*Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k2,
                        std::bind(&GraphicsApplication::toggleSkeletonView,
                                  this,
                                  SceneGraph::getRoot().get())); */

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k3,
                        std::bind(&GraphicsApplication::toggleWireframe,
                                  this)); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kC,
                        std::bind(&GraphicsApplication::toggleCamera,
                                  this)); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k1,
                        std::bind(&GraphicsApplication::toggleAABBDebug,
                                  this, 
                                  SceneGraph::getRoot())); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k1,
                        std::bind(&GraphicsApplication::toggleAABBDebug,
                                  this, 
                                  SceneGraph::getOctree())); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k4,
                        [&](){ m_drawMeshes = !m_drawMeshes; });

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kL,
                        std::bind(&GraphicsApplication::printSceneHierachy,
                                  this,
                                  SceneGraph::getRoot().get(),
                                  _T(""))); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kR, 
                        std::bind(&GraphicsApplication::recompileShaders,
                                  this));
  
  /*Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kJ, 
                        std::bind(&GraphicsApplication::toggleAnimation,
                                  this));*/

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kV,
                        [&](){ m_lockView = !m_lockView; });

  /*Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kUP,
                        std::bind(&GraphicsApplication::speedAnim, this, 1.f));

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kDOWN,
                        std::bind(&GraphicsApplication::speedAnim, this, -1.f));*/
}

void 
GraphicsApplication::loadResources() {
  
  m_currCam = 0;
  m_currAnim = 0;
  
  m_camNames[0] = _T("MAIN_CAM");
  m_camNames[1] = _T("UP_CAM");

  //m_animationsNames[0] = _T("Animation_mixamo.com");
  m_animationsNames[0] = _T("Animation_0");
  m_animationsNames[1] = _T("Animation_1");
  m_animationsNames[2] = _T("Animation_2");
  m_animationsNames[3] = _T("Animation_3");

  CameraManager::createCamera(m_camNames[0], 
                              {0, 100, 500}, 
                              {0, 100, 1}, 
                              m_viewport,
                              45, 0.1f, 10000.f);

  CameraManager::createCamera(m_camNames[1], 
                              {0, 100, -500}, 
                              {1, 100, 1}, 
                              m_viewport,
                              45, 0.1f, 10000.f);
  
  CameraManager::setActiveCamera(_T("MAIN_CAM"));
  
  ResourceManager::loadResource(_T("tiny_4anim.x"));

  ResourceManager::loadResource(_T("Spidey.fbx"));
  
  ResourceManager::loadResource(_T("Jump In Place.fbx"));
  
  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[0]);

  ResourceManager::loadResource(_T("Rifle Punch.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[1]);

  ResourceManager::loadResource(_T("Gunplay.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"),
                                  m_animationsNames[2]);

  ResourceManager::loadResource(_T("Hit_Reaction_shoot.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[3]);
  
  ResourceManager::loadResource(_T("HipHopDancing.fbx"));

  ResourceManager::loadResource(_T("Run.fbx"));

  ResourceManager::loadResource(_T("Unidad_1m.fbx"));
  
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));
    
  ResourceManager::loadResource(_T("script1.as"));

  ResourceManager::loadResource(_T("driderBehavior.as"));
}

void 
GraphicsApplication::createTechniques() {
  
  m_animTech = dr_make_unique<AnimationTechnique>();

  m_animTech->compile();

  m_staticTech = dr_make_unique<StaticMeshTechnique>();
  
  m_staticTech->compile();

  m_linesTech = dr_make_unique<LinesTechnique>();

  m_linesTech->compile();
}

void 
GraphicsApplication::createScene() {
  
  auto hip = addObject(_T("Grr"), _T("HipHopDancing.fbx"), true);

  hip->getTransform().setPosition({200, 0, 0});

  m_right = hip.get();

  auto hipNoAnim = addObject(_T("Grr2"), _T("HipHopDancing.fbx"), false);

  hipNoAnim->getTransform().setPosition({-200, 0, 0});

  auto hippy = addObject(_T("Grr3"), _T("HipHopDancing.fbx"), false);

  hippy->getTransform().setPosition({0, 0, 0});

  //hippy->getTransform().setRotation({0, Math::HALF_PI, 0});

  hippy->addChild(hip);

  m_center = hippy.get()
             ;

  /*auto cl = hipNoAnim->createInstance();

  *cl = *hipNoAnim;

  cl->getTransform().setPosition({-400, 0, 100});
  cl->getTransform().setRotation({-Math::HALF_PI, 0, 0});

  auto& cl2 = *cl->createInstance() = *cl;

  cl2.getTransform().setPosition({400, 0, 100});
  cl2.getTransform().setRotation({Math::HALF_PI, 0, 0});*/

  m_paths.resize(3);

  for (Int32 i = 0; i < 3; ++i) {
    m_pathRenders.emplace_back(m_paths[i]);
  }

  auto spidey = addObject(_T("ARA"), _T("Spidey.fbx"), true);

  spidey->createComponent<SpiderBehavior>();
  
  spidey->setParent(SceneGraph::createObject(_T("Spiders")));

  spidey->getTransform().rotate({0, Math::PI, 0});
  
  m_player = spidey.get();

  auto spiderAI = spidey->createInstance();

  *spiderAI = *spidey;

  spidey->createComponent<SpiderPlayer>();

  spidey->getTransform().setPosition({0.f, 0.f, 300.f});

  spiderAI->createComponent<SpiderAI>();

  m_spiderSpawn.setModel(spiderAI);

  /*auto cubo = ResourceManager::getReferenceT<Model>(_T("Unidad_1m.fbx"));
  //auto cubo = ResourceManager::getReferenceT<Model>(_T("tiny_4anim.x"));

  //auto cuboObj = addObjectFromModel(cubo, _T("CUBIN"));

  //cuboObj->getTransform().setScale({0.4f, 0.4f, 0.4f});
  ////cuboObj->getTransform().move({0, 0, 0});
  //cuboObj->getTransform().rotate({Degree(90).toRadian(), Degree(180).toRadian(), 0});

  //auto animSk = ResourceManager::getReferenceT<Skeleton>(cubo->skeletonName);

  //auto comp = cuboObj->createComponent<AnimatorComponent>();

  //comp->setBlendDuration(0.5f);

  //comp->setSkeleton(animSk);

  //m_anims[_T("Idle")] = cubo->animationsNames[0];
  //m_anims[_T("Walk")] = cubo->animationsNames[1];
  //m_anims[_T("Run")] = cubo->animationsNames[2];
  //m_anims[_T("Wave")] = cubo->animationsNames[3];

  //std::copy(std::begin(cubo->animationsNames), std::end(cubo->animationsNames), std::begin(m_playerAnims));

  //for (auto& p : cubo->animationsNames) {
  //  auto animA = ResourceManager::getReferenceT<Animation>(p);
  //  animA->setTicksPerSecond(4500.f);
  //  comp->addAnimation(animA, animA->getName());
  //}
  //
  //comp->setCurrentAnimation(cubo->animationsNames[0], false);
   

  /*auto terr = ResourceManager::getReferenceT<Model>(_T("nanosuit.obj"));

  auto terrainObj = addObjectFromModel(terr, _T("Terrain"));

  terrainObj->setStatic(true);

  terrainObj->getTransform().setPosition({0, 0, 550});

  terrainObj->getTransform().setScale({10, 10, 10});

 
  //auto womanNode = addObject(_T("LE Morrita"), _T("Run.fbx"), true);

  //womanNode->getTransform().setPosition({-500.f, 0, 200.f});

  //womanNode->getTransform().setScale({10.f, 10.f, 10.f});
 
  //
  //auto printComponents = [](GameObject* go) 
  //{
  //  auto cmps = go->getComponents<GameComponent>();
  //  Logger::addLog(_T("----------------"));
  //  for (auto& cmp : cmps) {
  //    Logger::addLog(cmp->getName());
  //  }
  //  Logger::addLog(_T("----------------"));
  //};

  ///*auto sc = ResourceManager::getReferenceT<ScriptCore>(_T("script1.as"));

  //auto scomp = womanNode->createComponent<ScriptComponent>(sc);

  //auto currentModule = ScriptEngine::instancePtr()->m_scriptEngine->GetModule("GameModule");
  //
  //auto result = currentModule->Build(); 

  //scomp->initScript();

  //scomp->start();
  //auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Gunplay.fbx"));

  //if (!walkerModel)
  //  return;

  //auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);
  //
  //auto walkerObj = addObjectFromModel(walkerModel, _T("LE Walker"));

  //auto animator = walkerObj->createComponent<AnimatorComponent>();

  //animator->setSkeleton(ws);

  //for (Int32 i = 0; i < (sizeof(m_animationsNames) / sizeof(TString)); ++i)
  //{
  //  auto wa = ResourceManager::getReferenceT<Animation>(m_animationsNames[i]);
  //  
  //  animator->addAnimation(wa, m_animationsNames[i]);
  //}

  //animator->setCurrentAnimation(m_animationsNames[m_currAnim], false);
  //
  //animator->setBlendDuration(1.f);
  //
  ////walkerObj->removeComponent<AnimatorComponent>();

  //walkerObj->getTransform().setPosition({300, 0, 450});

  //auto clone1 = walkerObj->createInstance();
  //*clone1 = *walkerObj;

  //clone1->getTransform().setPosition({150, 0, 300});

  //clone1->setName(_T("LE Walker 2"));

  //auto clone2 = walkerObj->createInstance();
  //*clone2 = *walkerObj;

  //clone2->getTransform().setPosition({450, 0, 150});

  //clone2->setName(_T("LE Walker 3"));
  ////walkerObj->getTransform().setScale({10.f, 10.f, 10.f});
  //  
  //m_right = walkerObj.get();

  //Int32 copies = 0;

  //std::mt19937 mt(std::random_device{}());
  //std::uniform_real_distribution<float> dt(-2000, 2000);
  //std::uniform_real_distribution<float> dt_A(10, 50);

  //for (Int32 i = 0; i < copies; ++i) {
  //  auto c = walkerObj->createInstance();
  //  *c = *walkerObj;
  //  c->getTransform().setPosition({dt(mt), 0, dt(mt)});
  //  c->getComponent<AnimatorComponent>()->setTime(dt_A(mt));
  //}

  //auto copy = walkerObj->createInstance();
  //*copy = *walkerObj;

  //copy->getTransform().move(-300, AXIS::kX);
  //copy->getTransform().setPosition(50, AXIS::kX);
  //copy->setName(_T("Player"));

  //auto sphereMod = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));*/

  auto sphereCenter = SceneGraph::createObject(_T("Spherin"));

  sphereCenter->getTransform().scale({20, 20, 20});
  
  m_cameraHolder = sphereCenter.get();
    
  auto camNode = SceneGraph::createObject(_T("Camera"));
  
  /*auto activeCam = CameraManager::getActiveCamera();

  camNode->createComponent<CameraComponent>(activeCam);

  camNode->getTransform().setPosition({0, 0, -20});

  camNode->setParent(sphereCenter);*/

  /*auto fd = m_player->createComponent<FrustumDebug>(activeCam.get());

  fd->setShaderTechnique(m_linesTech.get());*/
}

std::shared_ptr<GameObject>
GraphicsApplication::addObjectFromModel(std::shared_ptr<Model> model,
                                        const TString& name) {
  
  auto obj = SceneGraph::createObject(name);  
  
  if (!model) {
    Logger::addLog(_T("Trying to create object with null model"));
  }
  else {
    obj->createComponent<RenderComponent>(model);
    obj->createComponent<AABBCollider>(model->aabb);
  }

  return obj;
}

std::shared_ptr<GameObject> 
GraphicsApplication::addObject(const TString& name, 
                               const TString& model, 
                               bool animated) {

    auto obj = SceneGraph::createObject(name);  

    if (auto mod = ResourceManager::getReferenceT<Model>(model)) {

      obj->createComponent<RenderComponent>(mod);
      obj->createComponent<AABBCollider>(mod->aabb);

      if (animated && !mod->animationsNames.empty()) {

        auto animator = obj->createComponent<AnimatorComponent>();

        for (auto& anim : mod->animationsNames) {
          auto animation = ResourceManager::getReferenceT<Animation>(anim);
          animator->addAnimation(animation, anim);          
        }

        animator->setBlendDuration(0.7f);

        animator->setCurrentAnimation(mod->animationsNames[0], false);

        auto skel = ResourceManager::getReferenceT<Skeleton>(mod->skeletonName);

        animator->setSkeleton(skel);
      }
    }
    else {
      std::cout << "Model not found: " << StringUtils::toString(model) << std::endl;
    }

    return obj;
}

void 
GraphicsApplication::destroyModules() {

  m_animTech->destroy();
  m_staticTech->destroy();

  m_renderMan.exit();
  SceneGraph::shutDown();
  ResourceManager::shutDown();
  CameraManager::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  GraphicsDriver::shutDown();
  Logger::shutDown();
}

void 
GraphicsApplication::printSceneHierachy(GameObject* obj, const TString& msg) {

  Logger::addLog(msg + obj->getName());

  for (auto& child : obj->getChildren()) {
    printSceneHierachy(child.get(), msg + _T("\t"));
  }
}

void 
GraphicsApplication::toggleSkeletonView(GameObject* obj) {

  if (obj->getComponent<SkeletonDebug>()) {
    obj->removeComponent<SkeletonDebug>();
  }
  else if (obj->getComponent<AnimatorComponent>()) {
    auto skel = obj->createComponent<SkeletonDebug>();
    skel->setShaderTechnique(m_linesTech.get());
  }

  for (auto& child : obj->getChildren()) {
    toggleSkeletonView(child.get());
  }
}

void 
GraphicsApplication::toggleAABBDebug(std::shared_ptr<GameObject> obj) {
  
  if (!obj) {
    return;
  }

  if (obj->getComponent<AABBDebug>()) {
    obj->removeComponent<AABBDebug>();
  }
  else if (obj->getComponent<AABBCollider>()) {
    auto c = obj->createComponent<AABBDebug>(true);
    c->setShaderTechnique(m_linesTech.get());
  }

  for (auto& child : obj->getChildren()) {
    toggleAABBDebug(child);
  }

}

void 
GraphicsApplication::toggleWireframe() {
  static bool wire = false;

  static RasterizerState* rs = nullptr;

  if (rs) {
    rs->release();
    std::cout << "RS released" << std::endl;
  }

  auto desc = GraphicsAPI::getRasterizerState().getDescriptor();

  desc.fillMode = wire ? DR_FILL_MODE::kSolid : DR_FILL_MODE::kWireframe;

  rs = GraphicsAPI::getDevice().createRasteizerState(desc);

  rs->set(GraphicsAPI::getDeviceContext());

  wire = !wire;
}

void 
GraphicsApplication::toggleAnimation() {

  m_currAnim = (m_currAnim + 1) % (sizeof(m_animationsNames) / sizeof(TString));

  auto s = m_player->getComponent<AnimatorComponent>();
  s->blendAnimation(m_playerAnims[m_currAnim], true);

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->blendAnimation(m_animationsNames[m_currAnim], true);
      //animCmp->setTime(0);
    }
  }

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker 2"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->blendAnimation(m_animationsNames[m_currAnim], true);
      //animCmp->setTime(0);
    }
  }

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker 3"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->blendAnimation(m_animationsNames[m_currAnim], false);
      //animCmp->setTime(0);
    }
  }
}

void 
GraphicsApplication::speedAnim(float val) {

  auto names = {_T("LE Walker"), _T("LE Walker 2"), _T("LE Walker 3"), _T("LE Morrita")};

  for (auto& n : names) {
    if (auto obj = SceneGraph::getRoot()->findNode(n)) {
      if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
        animCmp->setSpeed(Math::max(animCmp->getSpeed() + val, 0.f));
      }
    }
  }
}

void 
GraphicsApplication::toggleCamera() {
  
  m_currCam = !m_currCam;

  CameraManager::setActiveCamera(m_camNames[m_currCam]);
}

void 
GraphicsApplication::initScriptEngine() {
  Int32 result;

  auto ctxMag = ContextManager::instancePtr();

  auto scriptEngine = ScriptEngine::instancePtr();

  //Create engine
  result = scriptEngine->createEngine();

  //Configurate engine
  result = scriptEngine->configurateEngine(ctxMag);

  //Register all functions
  result = Keyboard::registerFunctions(scriptEngine);
  Vector3D vector;
  result = vector.registerFunctions(scriptEngine);
  Transform transform;
  result = transform.registerFunctions(scriptEngine);
  result = Time::registerFunctions(scriptEngine);
  
  //Create a context
  scriptEngine->m_scriptContext = ctxMag->addContext(scriptEngine->m_scriptEngine,
                                                     _T("GameModule"));

  //Get script references of the ResourceManager
  auto bs = ResourceManager::getReferenceT<ScriptCore>(_T("driderBehavior.as"));

  //Add script section of behavior
  scriptEngine->addScript(bs->getName(),
                          bs->getScript(),
                          _T("GameModule"));

  
}

void 
GraphicsApplication::playerMovement() {
  
  if (!m_player) {
    return;
  }
    
  auto& playerPos = m_player->getTransform().getPosition();

  m_cameraHolder->getTransform().setPosition(playerPos);
  m_cameraHolder->getTransform().move(150, AXIS::kY);
}

void
GraphicsApplication::playerRotation() {

  static float rx = 0;
  static float ry = 0;

  const float cy = Degree(360).toRadian();
  const float cx = Degree(60).toRadian();

  auto delta = Mouse::getDisplacement();

  float sensitivity = 0.07f;
  float dx = delta.x * sensitivity * sensitivity; //* Time::getDelta();
  float dy = delta.y * sensitivity * sensitivity; //* Time::getDelta();

  //auto angles = m_cameraHolder->getTransform().getEulerAngles();
  
  //ry = Math::clamp(ry + dx, -cy, cy);
  ry += dx;
  rx = Math::clamp(rx + dy, -cx, cx);
 
  m_cameraHolder->getTransform().setRotation({rx, ry, 0});
  m_player->getTransform().setRotation({0, ry + Math::PI, 0});
}

std::vector<Vector3D> 
GraphicsApplication::calculatePoints() {
  std::vector<Vector3D> points(m_paths.size());

  float offset = 400.f;

  auto dir = Vector3D(m_player->getTransform().getRotation().vector2);

  auto left = dir.cross({0.f, 1.f, 0.f});

  left.normalize();

  auto& pos = m_player->getTransform().getPosition();

  points[0] = pos;

  for (SizeT i = 1; i < m_paths.size(); ++i) {
    points[i] = pos + left * offset;
    left *= -1;
  }

  return points;
}


// Inherited via Application
void 
GraphicsApplication::onResize() {

  GraphicsDriver::API().getSwapChain().resize(GraphicsDriver::API().getDevice(),
                                               m_viewport.width,
                                               m_viewport.height);
  CameraManager::getActiveCamera()->setViewport(m_viewport);
}

}