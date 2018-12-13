#include "dr_camera_component.h"

#include <dr_matrix3x3.h>
#include <dr_id_object.h>
#include "dr_camera_manager.h"

namespace driderSDK {

CameraComponent::CameraComponent(GameObject& _gameObject,
                                 SharedCamera _camera) 
  : GameComponent(_gameObject, _T("CameraComponent")) {
  onUpdate();
}

void 
CameraComponent::setActive() {
  CameraManager::setActiveCamera(m_camera.lock());
}

UInt32
CameraComponent::getWidth() {
  return m_camera.lock()->getViewportWidth();
}

void
CameraComponent::setWidth(UInt32 width) {
  Viewport newVp = m_camera.lock()->getViewport();
  newVp.width = width;
  m_camera.lock()->setViewport(newVp);
}

UInt32
CameraComponent::getHeight() {
  return m_camera.lock()->getViewportHeight();
}

void
CameraComponent::setHeight(UInt32 height) {
  Viewport newVp = m_camera.lock()->getViewport();
  newVp.height = height;
  m_camera.lock()->setViewport(newVp);
}

float
CameraComponent::getNearPlane() {
  return m_camera.lock()->getNearPlane();
}

void
CameraComponent::setNearPlane(float nearPlane) {
  m_camera.lock()->setNearPlane(nearPlane);
}

float
CameraComponent::getFarPlane() {
  return m_camera.lock()->getFarPlane();
}

void
CameraComponent::setFarPlane(float farPlane) {
  m_camera.lock()->setFarPlane(farPlane);
}

float
CameraComponent::getFov() {
  return m_camera.lock()->getFOV();
}

void
CameraComponent::setFov(float fov) {
  m_camera.lock()->setFOV(fov);
}

CameraViewType::E CameraComponent::getTypeCamera()
{
  return m_camera.lock()->typeCamera;
}

void
CameraComponent::onCreate() {
  Vector3D camPos = m_gameObject.getWorldTransform().getPosition();

  Matrix3x3 m3 = Matrix3x3(m_gameObject.getWorldTransform().getRotation());
  Vector3D camDir = m3.transpose() * Vector3D(0, 0, 1);

  Viewport vp;
  vp.topLeftX = 0;
  vp.topLeftY = 0;
  vp.width = 1280;
  vp.height = 720;

  float camFov = 60.f;
  float camNP = 0.1f;
  float camFP = 10000.0f;

  SizeT id = CameraManager::instance().getCameraCounter();

  m_camera =
  CameraManager::createCamera(_T("CM_CAM_") + StringUtils::toTString(id),
                              camPos,
                              camDir,
                              vp,
                              camFov,
                              camNP,
                              camFP);
}

void 
CameraComponent::onUpdate() {
  if (m_gameObject.changed()) {
    if (auto cam = m_camera.lock()) {
      auto& pos = m_gameObject.getWorldTransform().getPosition();
      Matrix3x3 m3 = Matrix3x3(m_gameObject.getWorldTransform().getRotation());
      auto dir =  m3.transpose() * Vector3D(0.0f, 0.0f, 1.0f);
      cam->setPosition(pos);
      cam->setTarget(pos + (dir * 50.f));
    }
  }
}

void 
CameraComponent::onRender() {
}

void 
CameraComponent::onDestroy() {
  CameraManager::deleteCamera(m_camera.lock()->getName());
}

UInt32
CameraComponent::getClassID() {
  UInt32 temp = CLASS_NAME_ID(CameraComponent);
  return temp;
}

GameComponent* 
CameraComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<CameraComponent>();
}

void CameraComponent::setTypeCamera(CameraViewType::E cameraType)
{
  m_camera.lock()->typeCamera = cameraType;
}

void
CameraComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Camera;
  file.m_file << StringUtils::toString(getName());

  
}

void
CameraComponent::deserialize(TString& data) {
  
}

}