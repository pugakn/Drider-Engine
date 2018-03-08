#include "dr_graphics_application.h"

#include <dr_aabb_collider.h>
#include <dr_camera_manager.h> 
#include <dr_device_context.h>
#include <dr_gameObject.h>
#include <dr_graph.h> //Scene graph
#include <dr_graphics_api.h> 
#include <dr_graphics_driver.h>
#include <dr_index_buffer.h>
#include <dr_input_manager.h>
#include <dr_model.h>
#include <dr_logger.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_vertex_buffer.h>
#include <dr_time.h>

#include "AnimationTechnique.h"
#include "StaticMeshTechnique.h"

namespace driderSDK {
GraphicsApplication::GraphicsApplication() {}
GraphicsApplication::~GraphicsApplication() {}
void
GraphicsApplication::postInit() {
  initModules();   
  loadResources();
  createScene();
}

void 
GraphicsApplication::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();
}


void 
GraphicsApplication::postRender() {

  GraphicsDriver::API().clear();

  Int32 queryFlags = QUERY_PROPERTY::kAny;

  auto& camera = *CameraManager::getActiveCamera();

  auto queryRes = SceneGraph::query(camera, 
                                    QUERY_ORDER::kBackToFront, 
                                    queryFlags);

  m_animTech->setCamera(&camera);
  m_staticTech->setCamera(&camera);

  auto& dc = GraphicsAPI::getDeviceContext();

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

void GraphicsApplication::loadResources() {
  
  CameraManager::createCamera(_T("MAIN_CAM"), 
                              {0, 200, -100}, 
                              {0, 150, 10}, 
                              m_viewport,
                              45, 0.1f, 4000.f);

  CameraManager::setActiveCamera(_T("MAIN_CAM"));

  ResourceManager::loadResource(_T("Croc.X"));

}

void GraphicsApplication::createScene() {
  
  auto crocModel = ResourceManager::getReferenceT<Model>(_T("Croc.X"));

  auto crocObj = addObjectFromModel(crocModel, _T("LE CROC"));

  crocObj->getTransform().setPosition({0, 100, 20});

  m_animTech = dr_make_unique<AnimationTechnique>();

  m_animTech->compile();

  m_staticTech = dr_make_unique<StaticMeshTechnique>();

  m_staticTech->compile();

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

}