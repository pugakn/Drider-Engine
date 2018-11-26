#include "dr_networkTransform_component.h"

#include <dr_vector3d.h>
#include <dr_gameObject.h>
#include <dr_file.h>

namespace driderSDK {

NetworkTransformComponent::NetworkTransformComponent(GameObject &_gameObject) :
                           GameComponent(_gameObject, _T("NetworkManagerComponent")) {}

NetworkTransformComponent::~NetworkTransformComponent() {

}

void
NetworkTransformComponent::onCreate() {

}

void
NetworkTransformComponent::onUpdate() {

}

void
NetworkTransformComponent::onDestroy() {

}

void
NetworkTransformComponent::onRender() {

}

UInt32
NetworkTransformComponent::getClassID() {
  return CLASS_NAME_ID(NetworkTransformComponent);
}

GameComponent*
NetworkTransformComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<NetworkTransformComponent>();
}

void
NetworkTransformComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::NetworkManager;
  file.m_file << StringUtils::toString(getName());
}

void
NetworkTransformComponent::deserialize(TString& data) {

}

void
NetworkTransformComponent::move(const Vector3D& distance) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::Move,
                          distance);
}

}