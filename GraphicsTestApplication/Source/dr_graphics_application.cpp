#include "dr_graphics_application.h"

#include <functional>

#include <dr_aabb_collider.h>
#include <dr_animation.h>
#include <dr_animator_component.h>
#include <dr_bone_attach_object.h>
#include <dr_camera_component.h>
#include <dr_camera_manager.h> 
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_gameObject.h>
#include <dr_graph.h> //Scene graph
#include <dr_graphics_api.h> 
#include <dr_graphics_driver.h>
#include <dr_index_buffer.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_joystick.h>
#include <dr_model.h>
#include <dr_logger.h>
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_skeleton.h>
#include <dr_vertex_buffer.h>
#include <dr_time.h>

#include "AABBDebug.h"
#include "AnimationTechnique.h"
#include "LinesTechnique.h"
#include "SkeletonDebug.h"
#include "StaticMeshTechnique.h"

namespace driderSDK {
GraphicsApplication::GraphicsApplication() : m_drawMeshes(true) {}
GraphicsApplication::~GraphicsApplication() {}
void
GraphicsApplication::postInit() {
  initModules();   
  initInputCallbacks();
  loadResources();
  createScene();
  createTechniques();

  Time::update();

  m_timer.init();
}

void 
GraphicsApplication::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();

  static Vector3D dir{0,0,200.f};

  m_left->getTransform().move(dir * Time::getDelta());

  m_right->getTransform().move(dir * Time::getDelta());

  if (m_timer.getSeconds() > 4.f) {
    dir *= -1;
    m_left->getTransform().rotate({0,Math::PI,0});
    m_right->getTransform().rotate({0,Math::PI,0});
    m_timer.init();
  }
}


void 
GraphicsApplication::postRender() {

  GraphicsDriver::API().clear();

  Int32 queryFlags = QUERY_PROPERTY::kAny;

  auto& camera = *CameraManager::getActiveCamera();

  m_animTech->setCamera(&camera);
  m_staticTech->setCamera(&camera);
  m_linesTech->setCamera(&camera);

  auto queryRes = SceneGraph::query(camera, 
                                    QUERY_ORDER::kBackToFront, 
                                    queryFlags);


  auto& dc = GraphicsAPI::getDeviceContext();

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  if (m_drawMeshes) {

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
  SceneGraph::startUp();
}

void 
GraphicsApplication::initInputCallbacks() {

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kS,
                        std::bind(&GraphicsApplication::toggleSkeletonView,
                                  this,
                                  SceneGraph::getRoot().get())); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kW,
                        std::bind(&GraphicsApplication::toggleWireframe,
                                  this)); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kA,
                        std::bind(&GraphicsApplication::toggleAABBDebug,
                                  this, 
                                  SceneGraph::getRoot().get())); 

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kM,
                        [&](){ m_drawMeshes = !m_drawMeshes; });

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kL,
                        std::bind(&GraphicsApplication::printSceneHierachy,
                                  this,
                                  SceneGraph::getRoot().get(),
                                  _T(""))); 
}

void 
GraphicsApplication::loadResources() {
  
  CameraManager::createCamera(_T("MAIN_CAM"), 
                              {0, 200, -400}, 
                              {0, 150, 10}, 
                              m_viewport,
                              45, 0.1f, 4000.f);

  CameraManager::setActiveCamera(_T("MAIN_CAM"));

  ResourceManager::loadResource(_T("Croc.X"));

  ResourceManager::loadResource(_T("Walking.fbx"));

  ResourceManager::loadResource(_T("Weapons-of-survival.fbx"));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));
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
  
  auto activeCam = CameraManager::getActiveCamera();
  
  auto crocModel = ResourceManager::getReferenceT<Model>(_T("Croc.X"));

  auto& crocAnimName = crocModel->animationsNames[0];

  auto ca = ResourceManager::getReferenceT<Animation>(crocAnimName);

  auto cs = ResourceManager::getReferenceT<Skeleton>(crocModel->skeletonName);

  auto crocObj = addObjectFromModel(crocModel, _T("LE CROC"));

  auto crocAnimator = crocObj->createComponent<AnimatorComponent>();

  crocAnimator->addAnimation(ca, crocAnimName);

  crocAnimator->setCurrentAnimation(crocAnimName);

  crocAnimator->setSkeleton(cs);

  //crocObj->getTransform().setPosition({0, 100, 20});

  auto walkerModel = ResourceManager::getReferenceT<Model>(_T("Walking.fbx"));

  auto& walkerAnimName = walkerModel->animationsNames[0];

  auto wa = ResourceManager::getReferenceT<Animation>(walkerAnimName);

  auto ws = ResourceManager::getReferenceT<Skeleton>(walkerModel->skeletonName);
  
  auto walkerObj = addObjectFromModel(walkerModel, _T("LE Walker"));

  auto animator = walkerObj->createComponent<AnimatorComponent>();

  animator->setSkeleton(ws);

  animator->addAnimation(wa, walkerAnimName);

  animator->setCurrentAnimation(walkerAnimName);

  walkerObj->getTransform().setPosition({300, 0, 200});

  m_right = walkerObj.get();

  auto copy = walkerObj->clone();

  copy->getTransform().move({-600}, AXIS::kX);
  
  m_left = copy.get();

  auto camHolder = SceneGraph::createObject(_T("Camera"));
  
  camHolder->createComponent<CameraComponent>(activeCam);

  camHolder->getTransform().setPosition({0, 200, -200});

  camHolder->setParent(copy);

  auto mazoMod = ResourceManager::getReferenceT<Model>(_T("Weapons-of-survival.fbx"));

  if (mazoMod) {
    auto mazo = SceneGraph::createObject<BoneAttachObject>(_T("Mazo 0"));

    mazo->createComponent<RenderComponent>(mazoMod);

    mazo->createComponent<AABBCollider>(mazoMod->aabb);

    mazo->setParent(walkerObj);

    mazo->setBoneAttachment(_T("RightHand"));

    mazo->getTransform().setScale({0.1f, 0.1f, 0.1f});

    auto mazo2 = std::dynamic_pointer_cast<BoneAttachObject>(mazo->clone());

    mazo2 ->setParent(copy);

    mazo2->setBoneAttachment(_T("LeftHand"));

    auto mazo3 = SceneGraph::createObject<BoneAttachObject>(_T("Mazo 3"));

    mazo3->createComponent<RenderComponent>(mazoMod);

    mazo3->createComponent<AABBCollider>(mazoMod->aabb);

    mazo3->setParent(crocObj);

    mazo3->getTransform().setScale({0.05f, 0.05f, 0.05f});

    mazo3->setBoneAttachment(_T("Bip01_R_Finger1"));
  }

  auto quadMod = ResourceManager::getReferenceT<Model>(_T("ScreenAlignedQuad.3ds"));

  auto quad = addObjectFromModel(quadMod, _T("Floor"));

  quad->getTransform().rotate({Math::HALF_PI, 0, 0});
  quad->getTransform().setScale({1000, 1000, 1000});
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
GraphicsApplication::toggleAABBDebug(GameObject* obj) {

  if (obj->getComponent<AABBDebug>()) {
    obj->removeComponent<AABBDebug>();
  }
  else if (obj->getComponent<AABBCollider>()) {
    auto c = obj->createComponent<AABBDebug>(true);
    c->setShaderTechnique(m_linesTech.get());
  }

  for (auto& child : obj->getChildren()) {
    toggleAABBDebug(child.get());
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

}