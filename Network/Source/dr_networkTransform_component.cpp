#include "dr_networkTransform_component.h"

#include <dr_vector3d.h>
#include <dr_gameObject.h>

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

TString
NetworkTransformComponent::serialize() {
  return L"Not implemented";
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