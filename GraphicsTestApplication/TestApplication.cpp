#include "TestApplication.h"
#include <dr_graphics_driver.h>
#include <unordered_map>
#include <random>
#include <iostream>
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
#include <dr_time.h>
#include <dr_gameObject.h>
#include <dr_material.h>
#include "DrawableComponent.h"
#include "ModelDebbug.h"
#include "NPCMovement.h"
#include "StaticMeshTechnique.h"
#include "LinesTechnique.h"
#include "CameraDebbug.h"
#include <dr_texture_core.h>

namespace driderSDK {

TestApplication::TestApplication() {}

TestApplication::~TestApplication() {}

void
TestApplication::postInit() {
  
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp((SizeT)m_hwnd);
  SceneGraph::startUp();
  Time::startUp();
  ResourceManager::startUp();
  
  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"), 
                                      m_viewport);

  m_camera->createProyection(45.f, 20.f, 3000.f);
  m_camera->getTransform().setPosition({0.f, 300.0f, -400});
  m_camera->setTarget({0.0f, 200.f, 1.0f});

  auto p = m_camera->createComponent<CameraDebbug>();

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

  initResources();
  initInput();
  initSceneGraph();
  
  m_animated.push_back(new Model3D());
  m_animated[0]->init(_T("HipHopDancing.fbx"));

  m_animated.push_back(new Model3D());
  m_animated[1]->init(_T("HipHopDancing.fbx"));
  m_animated[1]->transform.move({200, 0, 0});
  m_animated[1]->elapsedTime = 150.f;

  m_animated.push_back(new Model3D());
  m_animated[2]->init(_T("HipHopDancing.fbx"));
  m_animated[2]->transform.move({-200, 0, 0});
  m_animated[2]->elapsedTime = 33.f;
  
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
    m_joker->getTransform().rotate(Degree(90.f * Time::getDelta()), AXIS::kY);
  };

  auto rotateRight = [&]() 
  {
    m_joker->getTransform().rotate(Degree(-90.f * Time::getDelta()), AXIS::kY);
  };

  /*if (croc = m_sceneGraph->getRoot()->getChild(_T("Croc"))) {
    
    croc->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
    float scale = Math::cos(croc->transform.getRotation().y) * 0.5f + 1.f;
    croc->transform.setScale({scale,scale,scale}); 
  }*/

  if (auto node = SceneGraph::getRoot()->getChild(_T("Dwarf0"))) {
   
    node->getTransform().rotate(Degree(90 * Time::instance().getDelta()), 
                                AXIS::kY);
  }
  
  Vector3D dir = m_joker->getTransform().getDirection();
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
  }

  if (Keyboard::isKeyDown(KEY_CODE::kLEFT)) {
    rotateLeft();
  }

  else if (Keyboard::isKeyDown(KEY_CODE::kRIGHT)) {
    rotateRight();
  }

  float vel = 150.f * Time::getDelta();
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
  }
}

void
TestApplication::postUpdate() {
  
  InputManager::capture();
  Time::update();
  SceneGraph::update();

  input();
  for (auto& anim : m_animated) {
    anim->update();
  }
  
}

void 
TestApplication::postRender() {
  
  GraphicsDriver::API().clear();

  m_linesTech->setCamera(&(*m_activeCam));
  
  //SceneGraph::draw();
  
  for (auto& anim : m_animated) {
    anim->draw(*m_activeCam);
  }

  m_camera->render();

  m_technique->setCamera(&(*m_activeCam));

  auto dc = &GraphicsAPI::getDeviceContext();

  auto meshes = SceneGraph::query(*m_camera, 
                                  m_queryOrder, 
                                  QUERY_PROPERTYS::kOpaque | 
                                  QUERY_PROPERTYS::kDynamic | 
                                  QUERY_PROPERTYS::kStatic);

  dc->setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (auto& mesh : meshes) {

    m_technique->setWorld(&mesh.first);
    
    if (m_technique->prepareForDraw()) {
      if (auto material = mesh.second.material.lock()) {
        material->set();
      }
      mesh.second.vertexBuffer->set(*dc);
      mesh.second.indexBuffer->set(*dc);

      dc->draw(mesh.second.indicesCount, 0, 0);
    }
  }

  GraphicsDriver::API().swapBuffers();
}

void 
TestApplication::postDestroy() {

  for (auto& anim : m_animated) {
    anim->destroy();
    delete anim;
  }

  m_animated.clear();

  ResourceManager::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  SceneGraph::shutDown();
  GraphicsDriver::shutDown();
}

void addDrawableComponent(std::shared_ptr<driderSDK::GameObject> go,
                          Technique* tech) {
  if (go->getComponent<RenderComponent>()) {
    auto p = go->createComponent<ModelDebbug>();
    p->setShaderTechnique(tech);
  }
  for (auto child : go->getChildren())
  {
    addDrawableComponent(child, tech);
  }
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

  auto debugOctree = [&]() {
    auto octree = SceneGraph::getOctree();
    addDrawableComponent(octree, m_linesTech.get());
  };

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
                        debugOctree);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                         KEY_CODE::kJ, toggleWireframe);
}

void 
TestApplication::initResources() {
  
  //ResourceManager::loadResource(_T("Walking.fbx"));

  ResourceManager::loadResource(_T("Kachujin_diffuse.png"));

  ResourceManager::loadResource(_T("Croc.X"));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  ResourceManager::loadResource(_T("HipHopDancing.fbx"));

  ResourceManager::loadResource(_T("VenomJok.X"));

  ResourceManager::loadResource(_T("dwarf.x"));

  ResourceManager::loadResource(_T("DuckyQuacky_.fbx"));

}

void 
TestApplication::initSceneGraph() {
  
  auto createNode = [&](std::shared_ptr<GameObject> parent, 
                        const TString& name, 
                        const TString& resName,
                        const Vector3D& pos) {
  
    auto resource = ResourceManager::getReference(resName);

    auto model = std::dynamic_pointer_cast<Model>(resource);
    
    auto node = SceneGraph::createNode(parent, model);

    auto p = node->createComponent<ModelDebbug>();

    p->setShaderTechnique(m_linesTech.get());
  
    node->setName(name);

    node->getTransform().setPosition(pos);

    return node;
  };

  auto root = SceneGraph::getRoot();
     
  createNode(root, 
             _T("Quad"), 
             _T("ScreenAlignedQuad.3ds"), 
             {50, 300, 200})->getTransform().scale({40, 40, 40});

  std::unordered_map<Int32, TString> names
  {
    {0, _T("VenomJok.X")},
    {1, _T("Croc.X")},
    {2, _T("dwarf.x")},
    {3, _T("DuckyQuacky_.fbx")}
  };
 
  std::mt19937 mt(std::random_device{}());
  
  std::uniform_int_distribution<> dt(0, static_cast<Int32>(names.size() - 1));
  std::uniform_int_distribution<> scl(1, 3);
  std::uniform_real_distribution<float> space(-1500, 1500);

  for (Int32 i = 0; i < 50; ++i) {
    Vector3D pos(space(mt), 0, space(mt));
    TString aaa =StringUtils::toTString(i);

    auto mod = dt(mt);

    auto n = createNode(root, names[mod] + aaa, 
                        names[mod], 
                        pos);   
    n->setStatic(true);
    float sc = static_cast<float>(scl(mt));
    n->getTransform().scale({sc,sc,sc});
  }
  
  auto n = createNode(root, _T("Chinita"), _T("HipHopDancing.fbx"), {-200.f, 0.0f, 0.0f});
  n->getTransform().scale({ 1,1,1 });
  m_joker = n;
  
  auto chinitaMat = ResourceManager::createMaterial(_T("Mat_Chinita"));
  auto chinitaTex = ResourceManager::getReferenceT<TextureCore>(_T("Kachujin_diffuse.png"));

  chinitaMat->getProperty(_T("Albedo"))->texture = chinitaTex;
}

}