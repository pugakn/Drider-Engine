#include "dr_networkManager_component.h"

#include <dr_id_object.h>
#include <dr_gameObject.h>

namespace driderSDK {

NetworkManagerComponent::NetworkManagerComponent(GameObject &gameObject) :
                         GameComponent(gameObject, _T("NetworkManagerComponent")) {}

NetworkManagerComponent::~NetworkManagerComponent() {

}

void
NetworkManagerComponent::onCreate() {

}

void
NetworkManagerComponent::onUpdate() {

}


void
NetworkManagerComponent::onDestroy() {

}

void
NetworkManagerComponent::onRender() {

}

UInt32
NetworkManagerComponent::getClassID() {
  return CLASS_NAME_ID(NetworkManagerComponent);
}

GameComponent*
NetworkManagerComponent::cloneIn(GameObject&) {
  return 0;
}

void
NetworkManagerComponent::instantiate(GameObject& object,
                                     const Vector3D position,
                                     const Quaternion& rotation) {

}

void
NetworkManagerComponent::registerObject() {

}

void
NetworkManagerComponent::registerFloat(const TString& varName,
                                       float const& value) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::RegisterVar,
                          PARAM_TYPE::FLOAT,
                          varName,
                          value);
}

}