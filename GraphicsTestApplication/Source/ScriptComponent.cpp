#include "ScriptComponent.h"

namespace driderSDK {

ScriptComponent::~ScriptComponent() {

}

void
ScriptComponent::onCreate() {

}

void
ScriptComponent::onUpdate() {
  ScriptEngine* scriptEngine = ScriptEngine::instancePtr();
  
  Int32 result = scriptEngine->prepareFunction(_T("Update"));
  result = scriptEngine->executeCall();
}

/*********
/* TEMP */
void
ScriptComponent::onRender() {

}
/********/

void
ScriptComponent::onDestroy() {

}

void
ScriptComponent::addScript(TString name,
                           TString script) {

  Int32 result = ScriptEngine::instance().addScript(name,
                                                    script);
}

/**
* Clones the component inside the given gameObject
*/
void
ScriptComponent::cloneIn(GameObject& _go) {

}

void
ScriptComponent::start() {
  ScriptEngine *scriptEngine = ScriptEngine::instancePtr();

  Int32 result = scriptEngine->prepareFunction(_T("Start"));
  result = scriptEngine->executeCall();
}

}

