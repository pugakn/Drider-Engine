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
                                       float value) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::RegisterVar,
                          PARAM_TYPE::FLOAT,
                          varName,
                          value);
}
void
NetworkManagerComponent::registerInt(const TString& varName,
                                     Int32 value) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::RegisterVar,
                          PARAM_TYPE::INT,
                          varName,
                          value);
}

void
NetworkManagerComponent::registerString(const TString& varName,
                                        const TString& value) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::RegisterVar,
                          PARAM_TYPE::STRING,
                          varName,
                          value);
}


void
NetworkManagerComponent::instantiate(OBJ_TYPE::E objType,
                                     const Vector3D& position,
                                     const Vector3D& direction) {
  Messenger::sendFunction(m_gameObject.getName(),
                          FUNCTION_TYPE::Instantiate,
                          objType,
                          position,
                          direction);
}

}