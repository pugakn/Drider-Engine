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
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_skeleton.h>
#include <dr_script_component.h>
#include <dr_script_core.h>
#include <dr_script_engine.h>
#include <dr_vertex_buffer.h>
#include <dr_time.h>

#include "AABBDebug.h"
#include "AnimationTechnique.h"
#include "FrustumDebug.h"
#include "LinesTechnique.h"
#include "SkeletonDebug.h"
#include "StaticMeshTechnique.h"


namespace driderSDK {

GraphicsApplication::GraphicsApplication() 
  : m_drawMeshes(true), 
    m_lockView(true) 
{}

GraphicsApplication::~GraphicsApplication() {}

void
GraphicsApplication::postInit() {
  
  initModules();   
  initInputCallbacks();
  loadResources();
  initScriptEngine();
  createTechniques();
  createScene();

  Time::update();

  m_timer.init();
}

void 
GraphicsApplication::postUpdate() {
  Time::update();
  InputManager::update();

  static Vector3D dir{0,0,200.f};

  //m_right->getTransform().move(dir * Time::getDelta());
  
  if (m_timer.getSeconds() > 4.f) {
    dir *= -1;
    //m_right->getTransform().rotate({0,Math::PI,0});
    m_timer.init();
  }

  playerMovement();

  if (!m_lockView) {
    playerRotation();
  }

  SceneGraph::update();
}

void 
GraphicsApplication::postRender() {

  GraphicsDriver::API().clear();

  Int32 queryFlags = QUERY_PROPERTY::kAny;

  auto& camera = *CameraManager::getActiveCamera();

  m_animTech->setCamera(&camera);
  m_staticTech->setCamera(&camera);
  m_linesTech->setCamera(&camera);

  auto& mainC = CameraManager::getCamera(m_camNames[0]);
  
  auto& dc = GraphicsAPI::getDeviceContext();
  
  if (m_drawMeshes) {

    auto queryRes = SceneGraph::query(*mainC, 
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
}

void 
GraphicsApplication::initInputCallbacks() {
  
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k9,
                        &SceneGraph::buildOctree); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k2,
                        std::bind(&GraphicsApplication::toggleSkeletonView,
                                  this,
                                  SceneGraph::getRoot().get())); 

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
  
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kJ, 
                        std::bind(&GraphicsApplication::toggleAnimation,
                                  this));

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kV,
                        [&](){ m_lockView = !m_lockView; });
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
                              {0, 200, -400}, 
                              {0, 150, 10}, 
                              m_viewport,
                              45, 0.1f, 4000.f);

  CameraManager::createCamera(m_camNames[1], 
                              {0, 5000, 0}, 
                              {1, 0, 1}, 
                              m_viewport,
                              45, 0.1f, 10000.f);
  
  CameraManager::setActiveCamera(_T("MAIN_CAM"));

  ResourceManager::loadResource(_T("Croc.X"));

  ResourceManager::loadResource(_T("Jump In Place.fbx"));
  
  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[0]);

  ResourceManager::loadResource(_T("Shoot Rifle.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[1]);

  ResourceManager::loadResource(_T("Gunplay.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"),
                                  m_animationsNames[2]);

  ResourceManager::loadResource(_T("Hit_Reaction_shoot.fbx"));

  ResourceManager::renameResource(_T("Animation_mixamo.com"), 
                                  m_animationsNames[3]);

  ResourceManager::loadResource(_T("Run.fbx"));

  ResourceManager::loadResource(_T("Unidad_1m.fbx"));
  
  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  ResourceManager::loadResource(_T("Sphere.fbx"));
  
  ResourceManager::loadResource(_T("script1.as"));

  ResourceManager::loadResource(_T("MontiBehavior.as"));

  ResourceManager::loadResource(_T("nanosuit.obj"));
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
    
  auto cubo = ResourceManager::getReferenceT<Model>(_T("Unidad_1m.fbx"));

  auto cuboObj = addObjectFromModel(cubo, _T("CUBIN"));

  cuboObj->setStatic(true);

  auto terr = ResourceManager::getReferenceT<Model>(_T("nanosuit.obj"));

  auto terrainObj = addObjectFromModel(terr, _T("Terrain"));

  

  terrainObj->setStatic(true);

  terrainObj->clone()->getTransform().setPosition({0, 0, 550});

  auto woman = ResourceManager::getReferenceT<Model>(_T("Run.fbx"));

  auto woms = ResourceManager::getReferenceT<Skeleton>(woman->skeletonName);

  auto womanNode = addObjectFromModel(woman, _T("LE Morrita"));

  auto animatorW = womanNode->createComponent<AnimatorComponent>();

  animatorW->setSkeleton(woms);

  auto womAni = ResourceManager::getReferenceT<Animation>(woman->animationsNames[0]);

  animatorW->addAnimation(womAni, woman->animationsNames[0]);

  animatorW->setCurrentAnimation(woman->animationsNames[0], false, false);
  
  auto printComponents = [](GameObject* go) 
  {
    auto cmps = go->getComponents<GameComponent>();
    Logger::addLog(_T("----------------"));
    for (auto& cmp : cmps) {
      Logger::addLog(cmp->getName());
    }
    Logger::addLog(_T("----------------"));
  };

  auto sc = ResourceManager::getReferenceT<ScriptCore>(_T("script1.as"));

  auto scomp = womanNode->createComponent<ScriptComponent>(sc);

  auto currentModule = ScriptEngine::instancePtr()->m_scriptEngine->GetModule("GameModule");
  
  auto result = currentModule->Build(); 

  scomp->initScript();

  scomp->start();

  printComponents(womanNode.get());

  womanNode->getTransform().setPosition({-200.f, 0, 200.f});

  womanNode->getTransform().setScale({10.f, 10.f, 10.f});

  auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Gunplay.fbx"));

  auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);
  
  auto walkerObj = addObjectFromModel(walkerModel, _T("LE Walker"));

  auto animator = walkerObj->createComponent<AnimatorComponent>();

  animator->setSkeleton(ws);

  for (Int32 i = 0; i < (sizeof(m_animationsNames) / sizeof(TString)); ++i)
  {
    auto wa = ResourceManager::getReferenceT<Animation>(m_animationsNames[i]);
    
    animator->addAnimation(wa, m_animationsNames[i]);
  }

  animator->setCurrentAnimation(m_animationsNames[m_currAnim], false, false);
  
  //walkerObj->removeComponent<AnimatorComponent>();

  walkerObj->getTransform().setPosition({300, 0, 200});

  auto clone1 = walkerObj->clone();

  clone1->getTransform().setPosition({150, 0, 200});

  clone1->setName(_T("LE Walker 2"));

  clone1 = walkerObj->clone();

  clone1->getTransform().setPosition({450, 0, 200});

  clone1->setName(_T("LE Walker 3"));
  //walkerObj->getTransform().setScale({10.f, 10.f, 10.f});
    
  m_right = walkerObj.get();

  Int32 copies = 0;

  std::mt19937 mt(std::random_device{}());
  std::uniform_real_distribution<float> dt(-2000, 2000);
  std::uniform_real_distribution<float> dt_A(10, 50);

  for (Int32 i = 0; i < copies; ++i) {
    auto c = walkerObj->clone();
    c->getTransform().setPosition({dt(mt), 0, dt(mt)});
    c->getComponent<AnimatorComponent>()->setTime(dt_A(mt));
  }

  auto copy = walkerObj->clone();

  copy->getTransform().move(-300, AXIS::kX);

  auto sphereMod = ResourceManager::getReferenceT<Model>(_T("Sphere.fbx"));

  auto sphereCenter = SceneGraph::createObject(_T("Spherin"));

  sphereCenter->getTransform().scale({20, 20, 20});
  
  m_cameraHolder = sphereCenter.get();
  
  m_player = copy.get();
  
  auto camNode = SceneGraph::createObject(_T("Camera"));
  
  auto activeCam = CameraManager::getActiveCamera();

  camNode->createComponent<CameraComponent>(activeCam);

  camNode->getTransform().setPosition({0, 0, -20});

  camNode->setParent(sphereCenter);

  auto fd = copy->createComponent<FrustumDebug>(activeCam.get());

  fd->setShaderTechnique(m_linesTech.get());

  /*auto quadMod = ResourceManager::getReferenceT<Model>(_T("ScreenAlignedQuad.3ds"));

  auto quad = addObjectFromModel(quadMod, _T("Floor"));

  quad->getTransform().rotate({-Math::HALF_PI, 0, 0});
  quad->getTransform().setScale({10000, 10000, 10000});*/
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

void 
GraphicsApplication::destroyModules() {

  m_animTech->destroy();
  m_staticTech->destroy();

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

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->setCurrentAnimation(m_animationsNames[m_currAnim], true, true);
      //animCmp->setTime(0);
    }
  }

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker 2"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->setCurrentAnimation(m_animationsNames[m_currAnim], true, false);
      //animCmp->setTime(0);
    }
  }

  if (auto obj = SceneGraph::getRoot()->findNode(_T("LE Walker 3"))) {

    if (auto animCmp = obj->getComponent<AnimatorComponent>()) {
      animCmp->setCurrentAnimation(m_animationsNames[m_currAnim], false, false);
      //animCmp->setTime(0);
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
  auto bs = ResourceManager::getReferenceT<ScriptCore>(_T("MontiBehavior.as"));

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
  
  float forward = 0;
  float strafe = 0;

  const float velocity = 200.f;

  auto& worldMat = m_player->getTransform().getMatrix();
  auto direction = Vector3D(worldMat.vector2);
  auto left = direction.cross({0,1,0});

  direction.y = 0;
  left.y = 0;

  direction.normalize();
  left.normalize();

  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    strafe += velocity;
  }

  if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    strafe -= velocity;
  }

  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    forward += velocity;
  }

  if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    forward -= velocity;
  }  
  
  auto movement = (direction * forward + left * strafe) * Time::getDelta();

  m_player->getTransform().move(movement);

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

  float sensitivity = 0.08f;
  float dx = delta.x * 0.09f * Time::getDelta();
  float dy = delta.y * 0.09f * Time::getDelta();

  //auto angles = m_cameraHolder->getTransform().getEulerAngles();
  
  //ry = Math::clamp(ry + dx, -cy, cy);
  ry += dx;
  rx = Math::clamp(rx + dy, -cx, cx);
 
  m_cameraHolder->getTransform().setRotation({rx, ry, 0});
  m_player->getTransform().setRotation({0, ry, 0});
}

}