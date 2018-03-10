#include "TestApplication.h"

//Modules
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_input_manager.h>
#include <dr_logger.h>
#include <dr_resource_manager.h>
#include <dr_time.h>
#include <dr_radian.h>
#include <dr_model.h>
#include <dr_keyboard.h>

#include <dr_render_component.h>
#include <dr_aabb_collider.h>

namespace driderSDK {

void RenderManApp::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));
  ResourceManager::startUp();
  SceneGraph::startUp();
  Time::startUp();
  CameraManager::startUp();
  m_renderMan.init();

  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 100.0f, -200.0f },
                              { 0.0f, 50.f, 0.0f },
                              m_viewport,
                              45.f,
                              20.f,
                              3000.f
                             );
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  loadResources();

  auto model = SceneGraph::createObject(_T("Croc"));
  auto crocModel = ResourceManager::getReferenceT<Model>(_T("Croc.X"));
  if (crocModel) {
    model->createComponent<RenderComponent>(crocModel);
    model->createComponent<AABBCollider>(crocModel->aabb);
    model->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
  }
}

void
RenderManApp::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();


  static Vector3D dir{0, 0, 200.f};

  model->getTransform().move(dir * Time::getDelta());
}

void
RenderManApp::postRender() {
  GraphicsDriver::API().clear();
  m_renderMan.draw();
  GraphicsDriver::API().swapBuffers();
}

void
RenderManApp::postDestroy() {
  m_renderMan.exit();
  GraphicsDriver::shutDown();
  ResourceManager::shutDown();
  SceneGraph::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  Logger::shutDown();
}

void
RenderManApp::initInputCallbacks() {
  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kQ,
                        std::bind(&RenderManApp::RotateModelRight, this)); 
/*

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
*/
}

void
RenderManApp::RotateModelRight() {
  model->getTransform().setPosition({0.0f, 5.0f, 0.0f});
  model->update();
}

void RenderManApp::loadResources() {
  ResourceManager::loadResource(_T("Croc.X"));
}

}