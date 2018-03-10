#include "TestApplication.h"

//Modules
#include <dr_graphics_driver.h>
#include <dr_graph.h>
#include <dr_input_manager.h>
#include <dr_logger.h>
#include <dr_resource_manager.h>
#include <dr_time.h>

#include <dr_model.h>

#include <dr_render_component.h>
#include <dr_aabb_collider.h>

namespace driderSDK {

void RenderManApp::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp((SizeT)m_hwnd);
  ResourceManager::startUp();
  SceneGraph::startUp();
  Time::startUp();
  m_renderMan.init();

  loadResources();

  auto croc = SceneGraph::createObject(_T("Checker"));
  auto crocModel = ResourceManager::getReferenceT<Model>(_T("Checker.obj"));
  if (crocModel) {
    croc->createComponent<RenderComponent>(crocModel);
    croc->createComponent<AABBCollider>(crocModel->aabb);
  }
}

void RenderManApp::postUpdate() {
  Time::update();
  SceneGraph::update();
  InputManager::capture();
}

void RenderManApp::postRender() {
  GraphicsDriver::API().clear();
  m_renderMan.draw();
  GraphicsDriver::API().swapBuffers();
}

void RenderManApp::postDestroy() {
  m_renderMan.exit();
  GraphicsDriver::shutDown();
  ResourceManager::shutDown();
  SceneGraph::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  Logger::shutDown();
}

void RenderManApp::loadResources() {
  ResourceManager::loadResource(_T("Checker.obj"));
  ResourceManager::loadResource(_T("256_Checker_Diffuse.png"));
  ResourceManager::loadResource(_T("256_Checker_Displacement.png"));
  ResourceManager::loadResource(_T("256_Checker_Emissive.png"));
  ResourceManager::loadResource(_T("256_Checker_Metallic.png"));
  ResourceManager::loadResource(_T("256_Checker_Normal.png"));
  ResourceManager::loadResource(_T("256_Checker_Opacity.png"));
  ResourceManager::loadResource(_T("256_Checker_Roughness.png"));
  ResourceManager::loadResource(_T("256_Checker_Specular.png"));
  ResourceManager::loadResource(_T("256_Checker_SSColor.png"));
  ResourceManager::loadResource(_T("256_Checker_Thickness.png"));
}

}