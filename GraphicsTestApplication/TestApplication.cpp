#include "TestApplication.h"
#include <dr_graphics_driver.h>
#include <unordered_map>
#include <random>
#include <iostream>
#include <functional>
#include <dr_aabb_collider.h>
#include <dr_d3d_swap_chain.h>
#include <dr_degree.h>
#include <dr_gameObject.h>
#include <dr_input_manager.h>
#include <dr_joystick.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_quaternion.h>
#include <dr_radian.h>
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_string_utils.h>
#include <dr_skeleton.h>
#include <dr_texture_core.h>
#include <dr_time.h>
#include <dr_gameObject.h>
#include <dr_material.h>
#include <dr_animator_component.h>
#include <dr_bone_attach_object.h>
#include <dr_device_context.h>
#include <dr_device.h>
#include <dr_model.h>
#include <dr_graph.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include "DrawableComponent.h"
#include "AABBDebug.h"
#include "FrustumDebug.h"
#include "SkeletonDebug.h"
#include "StaticMeshTechnique.h"
#include "LinesTechnique.h"
#include "AnimationTechnique.h"

#include <dr_script_core.h>
#include "ScriptComponent.h"
#include "OneRef.h"

namespace driderSDK {

namespace KeyCode {
  enum E {
    ONE,
    TWO,
    THREE
  };
}

TestApplication::TestApplication() {}

TestApplication::~TestApplication() {}

void
TestApplication::postInit() {
  
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp((SizeT)m_hwnd);
  SceneGraph::startUp();
  Time::startUp();
  ResourceManager::startUp();
  ScriptEngine::startUp();
  
  m_queryOrder = QUERY_ORDER::kFrontToBack;

  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"), 
                                      m_viewport);

  m_camera->createProyection(45.f, 20.f, 3000.f);
  m_camera->getTransform().setPosition({0.f, 300.0f, -400});
  m_camera->setTarget({0.0f, 200.f, 1.0f});

  auto p = m_camera->createComponent<FrustumDebug>();

  m_linesTech = dr_make_unique<LinesTechnique>(&(*m_activeCam), 
                                 &m_camera->getWorldTransform().getMatrix());

  m_linesTech->compile();

  p->setShaderTechnique(m_linesTech.get());

  m_leftCam = std::make_shared<Camera>(_T("LEFT_CAM"), 
                                        m_viewport);
  
  
  m_leftCam ->createProyection(45.f, 0.1f, 10000.f);
  m_leftCam->getTransform().setPosition({-4000.f, 0000.f, 1000.f});
  m_leftCam->setTarget({0.f, 0.f, 1000.f});

  m_upCam = std::make_shared<Camera>(_T("UP_CAM"), 
                                        m_viewport);
  
  
  m_upCam ->createProyection(45.f, 0.1f, 10000.f);
  m_upCam->getTransform().setPosition({0.f, 4000.f, 0.f});
  m_upCam->setTarget({1.f, 1.f, 200.f});

  m_activeCam = m_leftCam;

  m_technique = dr_make_unique<StaticMeshTechnique>();

  m_technique->compile();

  m_animTech = dr_make_unique<AnimationTechnique>();

  m_animTech->compile();

  initResources();
  
  /*m_animated.push_back(new Model3D());
  m_animated[0]->init(_T("HipHopDancing.fbx"));

  m_animated.push_back(new Model3D());
  m_animated[1]->init(_T("HipHopDancing.fbx"));
  m_animated[1]->transform.move({200, 0, 0});
  m_animated[1]->elapsedTime = 150.f;

  m_animated.push_back(new Model3D());
  m_animated[2]->init(_T("HipHopDancing.fbx"));
  m_animated[2]->transform.move({-200, 0, 0});
  m_animated[2]->elapsedTime = 33.f;*/

  initInput();
  initSceneGraph();
  initScriptEngine();

  /*SceneGraph::addObject(m_leftCam);
  */SceneGraph::addObject(m_upCam);
  m_leftCam->setParent(m_joker);
  //m_upCam->setParent(m_joker);
  m_joker->addChild(m_camera);

  //m_sceneGraph->query(*m_camera, QUERY_ORDER::kBackToFront, 0);
  
  /*result = FMOD::System_Create(&system);
  result = system->getVersion(&version);
  if (version < FMOD_VERSION) {
    return;
  }

  result = system->init(32, FMOD_INIT_NORMAL, 0);
  result = system->createSound("testSound.mp3", FMOD_DEFAULT, 0, &sound1);
  result = sound1->setMode(FMOD_LOOP_OFF);*/

  m_debugList = false;

}

void
TestApplication::input() {
    
  GameObject::SharedGameObj croc;

  auto rotateLeft = [&]() 
  {
    m_joker->getTransform().rotate(Vector3D{0, Math::PI * Time::getDelta(),0});
  };

  auto rotateRight = [&]() 
  {
    m_joker->getTransform().rotate(Vector3D{0, -Math::PI * Time::getDelta(),0});
  };

  auto rot = [&](const Vector3D& angl)
  {
    m_wep->getTransform().rotate(angl);
  };
    
  /*Vector3D dir = m_joker->getTransform().getDirection();
  Vector3D right = dir.cross(Vector3D(0,1,0));
  float f = 0;
  float s = 0;

  float fc = 0;
  float sc = 0;

  if (Joystick::get(0)) {   
    
    f = -Joystick::get(0)->getAxis(JOYSTICK_AXIS::kLSVer);
    if (Math::abs(f) < 0.1f) {
      f = 0.0f;
    }

    s = -Joystick::get(0)->getAxis(JOYSTICK_AXIS::kLSHor);
    if (Math::abs(s) < 0.1f) {
      s = 0.0f;
    }

  }

  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    f = 1;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    f = -1;
  }

  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    s = 1;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    s = -1;
  }*/

  if (Keyboard::isKeyDown(KEY_CODE::kLEFT)) {
    rotateLeft();
  }

  else if (Keyboard::isKeyDown(KEY_CODE::kRIGHT)) {
    rotateRight();
  }

  if (Keyboard::isKeyDown(KEY_CODE::kY)) {
    rot({0,Math::PI * Time::getDelta(),0});
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kZ)) {
    rot({0,0,Math::PI * Time::getDelta()});
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kX)) {
    rot({Math::PI * Time::getDelta(),0, 0});
  }


  /*float vel = 150.f * Time::getDelta();
  float camVel = 500.f * Time::getDelta();
 
  if (Math::abs(f) > 0.0f || Math::abs(s) > 0.0f) {

    if (m_upCam != m_activeCam) {
      m_joker->getTransform().move((dir * f + right * s) * vel);
    }
    else {
      m_upCam->getTransform().move((Vector3D{0,0,1} * f + Vector3D{-1,0,0} * s) * camVel);
      auto target = m_upCam->getTransform().getPosition();
      m_upCam->setTarget({target.x + 0.5f, 0.f, target.z + 0.5f});
    }
  }*/
}

void
TestApplication::postUpdate() {
  
  InputManager::capture();
  Time::update();
  SceneGraph::update();

  input();
  
}

void 
TestApplication::postRender() {
  
  GraphicsDriver::API().clear();

  m_linesTech->setCamera(&(*m_activeCam));
  
  //SceneGraph::draw();

  //Show Frustum
  m_camera->render();
  
  m_animTech->setCamera(&(*m_activeCam));
  m_technique->setCamera(&(*m_activeCam));

  auto dc = &GraphicsAPI::getDeviceContext();

  auto queryRes = SceneGraph::query(*m_camera, 
                                  m_queryOrder, 
                                  QUERY_PROPERTY::kOpaque | 
                                  QUERY_PROPERTY::kDynamic | 
                                  QUERY_PROPERTY::kStatic);

  dc->setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (auto& queryObj : queryRes) {

    auto tech = m_technique.get();

    if (queryObj.bones) {
      tech = m_animTech.get();
      m_animTech->setBones(*queryObj.bones);
    }

    tech->setWorld(&queryObj.world);
    
    if (tech->prepareForDraw()) {
      if (auto material = queryObj.mesh.material.lock()) {
        material->set();
      }
      queryObj.mesh.vertexBuffer->set(*dc);
      queryObj.mesh.indexBuffer->set(*dc);

      dc->draw(queryObj.mesh.indicesCount, 0, 0);
    }
  }

  GraphicsDriver::API().swapBuffers();
}

void 
TestApplication::postDestroy() {
  
  m_technique->destroy();
  m_linesTech->destroy();
  m_animTech->destroy();

  ResourceManager::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  SceneGraph::shutDown();
  GraphicsDriver::shutDown();
  Logger::shutDown();
}

void 
TestApplication::initInput() {
 
  auto toggleCam = [&]()
  {
    if (m_activeCam == m_leftCam) {
      m_activeCam = m_upCam;
    }
    else if(m_activeCam == m_upCam) {
      m_activeCam = m_camera;
    }
    else {
      m_activeCam = m_leftCam;
    }
  };

  auto root = SceneGraph::getRoot();
  
  auto toggleWireframe = [&]()
  {
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
  };

  auto debugList = [&](){
    m_debugList = true;
  };

  auto toggleQueryOrder = [&]() {
    m_queryOrder = static_cast<QUERY_ORDER::E>(!m_queryOrder);
  };
  

  auto bindPH = std::bind(&TestApplication::printHerarchy, 
                          this, 
                          SceneGraph::getRoot(), 
                          _T(""));

  auto bindTAB = std::bind(&TestApplication::toggleAABBDebug,
                           this, 
                           SceneGraph::getRoot());

  auto bindTSD = std::bind(&TestApplication::toggleSkeletonDebug,
                           this, 
                           SceneGraph::getRoot());

  auto clone = [&]()
  {
    std::mt19937 mt(std::random_device{}());
  
    std::uniform_real_distribution<float> space(-300, 300);

    auto c = m_joker->clone();
    SceneGraph::addObject(c);
    c->getTransform().move({space(mt), 0, space(mt)});
  };

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kC,
                        clone);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kH,
                        bindPH);
    
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kQ,
                        toggleQueryOrder);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kT,
                        toggleCam);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kL,
                        debugList);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kO,
                        SceneGraph::buildOctree);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kP,
                        bindTAB);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kK,
                        bindTSD);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                         KEY_CODE::kJ, toggleWireframe);
    
  auto keyPressed = [&]() {
    return true;
  };
  
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::k0, keyPressed);
}

void 
TestApplication::initResources() {
  
  //ResourceManager::loadResource(_T("Walking.fbx"));

  ResourceManager::loadResource(_T("Kachujin_diffuse.png"));

  ResourceManager::loadResource(_T("Croc.X"));

  ResourceManager::loadResource(_T("Cube.fbx"));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  ResourceManager::loadResource(_T("Walking.fbx"));

  ResourceManager::loadResource(_T("Weapons-of-survival.fbx"));

  ResourceManager::loadResource(_T("test.as"));
}

void 
TestApplication::initSceneGraph() {
  
  auto createNode = [&](const TString& name, 
                        const TString& resName,
                        const Vector3D& pos) {
  
    auto resource = ResourceManager::getReference(resName);

    auto model = std::dynamic_pointer_cast<Model>(resource);
    
    auto node = SceneGraph::createObject(name);

    node->createComponent<RenderComponent>(model);

    node->createComponent<AABBCollider>(model->aabb);

    if (!model->animationsNames.empty()) {
      
      auto animName = model->animationsNames[0];
      
      auto animation = ResourceManager::getReferenceT<Animation>(animName);

      auto skel = ResourceManager::getReferenceT<Skeleton>(model->skeletonName);

      auto anim = node->createComponent<AnimatorComponent>();

      anim->setSkeleton(skel);

      anim->addAnimation(animation, animName);

      anim->setCurrentAnimation(animName);

    }
      
    node->getTransform().setPosition(pos);

    return node;
  };

  auto root = SceneGraph::getRoot();
     
  std::unordered_map<Int32, TString> names
  {
    {0, _T("Walking.fbx")},
    {1, _T("Croc.X")},
  };
  
  auto chinitaMat = ResourceManager::createMaterial(_T("Mat_Chinita"));

  auto chinitaTex = ResourceManager::getReferenceT<TextureCore>(_T("Kachujin_diffuse.png"));

  chinitaMat->getProperty(_T("Albedo"))->texture = chinitaTex;

  auto n = createNode(_T("Chinita"), _T("Walking.fbx"), {-200.f, 0.0f, 0.0f});
  
  n->getTransform().scale({ 1,1,1 });
  
  n->getComponent<RenderComponent>()->getMeshes()[0].material = chinitaMat;

  m_joker = n;
  
  auto cube = SceneGraph::createObject<BoneAttachObject>(_T("Cube"));
  
  auto cubeMod = ResourceManager::getReferenceT<Model>(_T("Weapons-of-survival.fbx"));
  
  cube->createComponent<RenderComponent>(cubeMod);

  cube->createComponent<AABBCollider>(cubeMod->aabb);

  cube->setParent(n);

  cube->setBoneAttachment(_T("RightHand"));

  cube->getTransform().scale({0.1f, 0.1f, 0.1f});

  m_wep = cube;
  
  auto ss = createNode(_T("Mother"), _T("Walking.fbx"), {200.f, 0.0f, 400.0f});

  ss->getTransform().rotate({Math::PI,0,0});

  n->addChild(ss);

  
  
  std::mt19937 mt(std::random_device{}());
  
  std::uniform_int_distribution<> dt(0, static_cast<Int32>(names.size() - 1));
  std::uniform_int_distribution<> scl(1, 3);
  std::uniform_real_distribution<float> space(-1500, 1500);
  std::uniform_real_distribution<float> time(0, 1000);

  for (Int32 i = 0; i < 0; ++i) {
    
    auto pp = n->clone();
    
    pp->setName(n->getName() + _T("(clone)"));
    pp->getTransform().setPosition({space(mt), 0, space(mt)});
    
    auto an = pp->getComponent<AnimatorComponent>();
    an->setTime(time(mt));
  }
}

void TestApplication::initScriptEngine() {
  int result;

  //scriptEngine->addScriptLog(_T("hola"), 0);
  ScriptEngine* scriptEngine = nullptr;

  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
  }

  scriptEngine = ScriptEngine::instancePtr();

  result = scriptEngine->createEngine(); 

  result = Keyboard::registerFunctions(scriptEngine);

  Vector3D vector;
  result = vector.registerFunctions(scriptEngine);

  /*Object obj;
  result = scriptEngine->m_scriptEngine->RegisterObjectType("Object",
  sizeof(Object),
  asOBJ_VALUE | asOBJ_APP_CLASS |
  asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR |
  asOBJ_APP_CLASS_DESTRUCTOR);

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
  asBEHAVE_CONSTRUCT,
  "void f()",
  asFUNCTION(Constructor),
  asCALL_CDECL_OBJLAST);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
  asBEHAVE_CONSTRUCT,
  "void f(const Object& in)",
  asFUNCTION(CopyConstruct),
  asCALL_CDECL_OBJLAST);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
  asBEHAVE_CONSTRUCT,
  "void f(float, float)",
  asFUNCTION(ConstructFromTwoFloats),
  asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Object",
  asBEHAVE_DESTRUCT,
  "void f()",
  asFUNCTION(Destructor),
  asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
  "Object& opAssign(const Object& in)",
  asMETHODPR(Object, operator=, (const Object&), Object&),
  asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
  "Object opAdd(const Object& in) const",
  asMETHODPR(Object, operator+, (const Object&) const, Object),
  asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
  "Object& opAddAssign(const Object& in)",
  asMETHODPR(Object, operator+=, (const Object&), Object&),
  asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
  "Object& add(const Object& in)",
  asMETHODPR(Object, add, (const Object&), Object&),
  asCALL_THISCALL);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Object",
  "Object& si()",
  asMETHOD(Object, si, Object&),
  asCALL_THISCALL);*/
  //result = m_camera->registerFunctions(scriptEngine);
  
  result = m_activeCam->getTransform().registerFunctions(scriptEngine);
  result = scriptEngine->m_scriptEngine->RegisterGlobalProperty("Transform transform",
                                                                &m_activeCam->getTransform());


  result = scriptEngine->configureContext();

  auto camScript = m_camera->createComponent<ScriptComponent>();
  auto script = std::dynamic_pointer_cast<ScriptCore>(ResourceManager::getReference(_T("test.as")));
  camScript->addScript(_T("test.as"), script->getScript());

  result = scriptEngine->compileScript();
  camScript->start();
}

void 
TestApplication::printHerarchy(std::shared_ptr<GameObject> obj, 
                                    const TString & off) {
    Logger::addLog(off + obj->getName());

    for (auto& child : obj->getChildren()) {
      printHerarchy(child, off + _T("\t"));
    }
}

void 
TestApplication::toggleAABBDebug(std::shared_ptr<GameObject> go) {
  if (auto collider = go->getComponent<AABBCollider>()) {

    if (go->getComponent<AABBDebug>()) {
      
      go->removeComponent<AABBDebug>();

    }

    else {

      auto p = go->createComponent<AABBDebug>(true);
      p->setShaderTechnique(m_linesTech.get());

    }
  }
  for (auto child : go->getChildren())
  {
    toggleAABBDebug(child);
  }
}

void 
TestApplication::toggleSkeletonDebug(std::shared_ptr<GameObject> go) {
  if (auto re = go->getComponent<AnimatorComponent>()) {

    if (go->getComponent<SkeletonDebug>()) {
      
      go->removeComponent<SkeletonDebug>();

    }
    else {
      auto p = go->createComponent<SkeletonDebug>();
      p->setShaderTechnique(m_linesTech.get());
    }
  }

  for (auto child : go->getChildren())
  {
    toggleSkeletonDebug(child);
  }
}

void
TestApplication::addScript(TString name) {
  auto script = std::dynamic_pointer_cast<ScriptCore>
    (ResourceManager::getReference(name));

  Int32 result = ScriptEngine::instance().addScript(name,
                                                    script->getScript());
}

}