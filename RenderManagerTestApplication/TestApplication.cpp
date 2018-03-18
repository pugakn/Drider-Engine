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
#include <dr_mouse.h>

#include <dr_render_component.h>
#include <dr_aabb_collider.h>
#include <dr_degree.h>

namespace driderSDK {

void
HSVtoRGB(float fH, float fS, float fV,
         float& fR, float& fG, float& fB) {
  float fC = fV * fS;
  float fX = fC * (1.0f - abs(fmod((fH / 60.0f), 2) - 1.0f));
  float fM = fV - fC;

  fR = fM;
  fG = fM;
  fB = fM;

  if (fH < 60.0f) {
    fR += fC;
    fG += fX;
    //fB += 0.0f;
    return;
  }
  else if (fH < 120.0f) {
    fR += fX;
    fG += fC;
    //fB += 0.0f;
    return;
  }
  else if (fH < 180.0f) {
    //fR += 0.0f;
    fG += fC;
    fB += fX;
    return;
  }
  else if (fH < 240.0f) {
    //fR += 0.0f;
    fG += fX;
    fB += fC;
    return;
  }
  else if (fH < 300.0f) {
    fR += fX;
    //fG += 0.0f;
    fB += fC;
    return;
  }
  else if (fH <= 360.0f) {
    fR = fC;
    //fG = 0.0f;
    fB = fX;
    return;
  }
}

void
RenderManApp::postInit() {
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

  Degree grados(2.8125);
  Vector4D LightPosition(0, 50, -50, 1);
  Matrix4x4 rotationMatrix(driderSDK::Math::FORCE_INIT::kIdentity);
  rotationMatrix.RotationY(grados.toRadian());

  float lighIndex = 0.0f;
  for (int i = 0; i < 128; ++i) {
    Lights[i].m_vec4Position = LightPosition;
    LightPosition = rotationMatrix * LightPosition;

    Lights[i].m_fIntensity = 1.0f;
    HSVtoRGB(lighIndex*256, 1.0f, 1.0f,
             Lights[i].m_vec4Color.x, Lights[i].m_vec4Color.y, Lights[i].m_vec4Color.z);
    Lights[i].m_vec4Color.w = 1.0f;
    lighIndex += 1.0f/128.0f;
  }

  m_renderMan.lights = &Lights;

  CameraManager::createCamera(_T("PATO_CAM"),
                              { 0.0f, 100.0f, -200.0f },
                              { 0.0f, 50.f, 0.0f },
                              m_viewport,
                              45.f,
                              20.f,
                              3000.f
                             );
  CameraManager::setActiveCamera(_T("PATO_CAM"));

  modelMovement = Vector3D(0.0f, 0.0f, 0.0f);

  loadResources();

  model = SceneGraph::createObject(_T("Checker"));
  auto crocModel = ResourceManager::getReferenceT<Model>(_T("Croc.X"));
  if (crocModel) {
    model->createComponent<RenderComponent>(crocModel);
    model->createComponent<AABBCollider>(crocModel->aabb);
    model->getTransform().setPosition(Vector3D(0.0f, 0.0f, 0.0f));
  }

  initInputCallbacks();
}

void
RenderManApp::postUpdate() {
  Time::update();
  InputManager::update();
  SceneGraph::update();

  const float fMovementSpeed = 50.0f;
  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    model->getTransform().move(Vector3D(1.0f, 0.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    model->getTransform().move(Vector3D(-1.0f, 0.0f, 0.0f) * Time::getDelta()* fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    model->getTransform().move(Vector3D(0.0f, 0.0f, 1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    model->getTransform().move(Vector3D(0.0f, 0.0f, -1.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kQ)) {
    model->getTransform().move(Vector3D(0.0f, 1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }
  if (Keyboard::isKeyDown(KEY_CODE::kE)) {
    model->getTransform().move(Vector3D(0.0f, -1.0f, 0.0f) * Time::getDelta() * fMovementSpeed);
  }

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
  Mouse::addMovedCallback(std::bind(&RenderManApp::RotateModel, this));
}

void
RenderManApp::RotateModel() {
  Vector2DI mouseDelta = Mouse::getDisplacement();
  Vector3D rotation{0.0f, 0.0f, 0.0f};

  if (Mouse::isButtonDown(MOUSE_BUTTON::kLeft)) {
    if (Math::abs(mouseDelta.x) < 3.0f) {
      mouseDelta.x = 0.0f;
    }
    if (Math::abs(mouseDelta.y) < 3.0f) {
      mouseDelta.y = 0.0f;
    }

    rotation.y = -mouseDelta.x;
    rotation.x = -mouseDelta.y;

    model->getTransform().rotate(rotation * Time::getDelta());
  }
}

void
RenderManApp::MoveModel(Vector3D direction) {
  modelMovement += direction;
}

void
RenderManApp::loadResources() {
  ResourceManager::loadResource(_T("Croc.X"));
}

}