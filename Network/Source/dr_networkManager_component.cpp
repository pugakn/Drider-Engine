#include "dr_networkManager_component.h"

#include <dr_id_object.h>

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
  Messenger::sendFunction(this->ID(),
                          FUNCTION_TYPE::Instantiate,
                          &object, position, rotation);
}




}