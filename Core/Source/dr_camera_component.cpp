#include "dr_camera_component.h"

#include "dr_camera.h"
#include "dr_camera_manager.h"

namespace driderSDK {

CameraComponent::CameraComponent(GameObject& _gameObject,
                                 SharedCamera _camera) 
  : GameComponent(_gameObject)
{
  setCamera(_camera);
}

void 
CameraComponent::setCamera(SharedCamera camera) {
  m_camera = camera;

  if (auto cam = m_camera.lock()) {
      cam->setPosition(m_gameObject.getWorldTransform().getPosition());
  }
}

void 
CameraComponent::setActive() {
  CameraManager::setActiveCamera(m_camera.lock());
}

void
CameraComponent::onCreate() {
}

void 
CameraComponent::onUpdate() {

  if (m_gameObject.changed()) {
    if (auto cam = m_camera.lock()) {
      auto& pos = m_gameObject.getWorldTransform().getPosition();
      auto& dir = m_gameObject.getWorldTransform().getDirection();
      cam->setPosition(pos);
      cam->setTarget(pos + dir * 50.f);
    }
  }
}

void 
CameraComponent::onRender() {
}

void 
CameraComponent::onDestroy() {
}

void 
CameraComponent::cloneIn(GameObject&) {

}

}