#include "dr_script_component.h"
#include <functional>

#include <..\..\Script\Include\dr_context_manager.h>
#include <..\..\Script\Include\dr_script_engine.h>

#include "dr_script_core.h"
#include <dr_gameObject.h>

namespace driderSDK {

ScriptComponent::~ScriptComponent() {

}

void
ScriptComponent::onCreate() {
  
  scriptEngine = ScriptEngine::instancePtr();

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
                              std::bind(&ScriptComponent::onKeyDown, this, std::placeholders::_1));

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyReleased,
                              std::bind(&ScriptComponent::onKeyUp, this, std::placeholders::_1));
  
  //Get module
  mod = scriptEngine->m_scriptEngine->GetModule("GameModule");
  //Get script object
  scriptEngine->getScriptObject(_T("script1"),
                                mod,
                                &obj,
                                &type);
  
  start();
}

void
ScriptComponent::onUpdate() {
  scriptEngine->setObjectToScript(type,
                                  _T("void SetTransform(Transform@ trans)"),
                                  0,
                                  &m_gameObject.getTransform(),
                                  obj);

  scriptEngine->executeFunction(_T("void Update()"),
                                type,
                                obj);
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
                           TString script,
                           TString module) {

  Int32 result = ScriptEngine::instance().addScript(name,
                                                    script,
                                                    module);

}

/**
* Clones the component inside the given gameObject
*/
void
ScriptComponent::cloneIn(GameObject& _go) {

}

void
ScriptComponent::start() {
  scriptEngine->setObjectToScript(type,
                                  _T("void SetTransform(Transform@ trans)"),
                                  0,
                                  &m_gameObject.getTransform(),
                                  obj);

  scriptEngine->executeFunction(_T("void Start()"),
                                type,
                                obj);
}

void
ScriptComponent::onKeyDown(KEY_CODE::E key) {
  scriptEngine->setObjectToScript(type,
                                  _T("void SetTransform(Transform@ trans)"),
                                  0,
                                  &m_gameObject.getTransform(),
                                  obj);

  scriptEngine->executeFunctionParam(_T("void onKeyDown(KeyCode)"),
                                     type,
                                     obj,
                                     key);
}

void
ScriptComponent::onKeyUp(KEY_CODE::E key) {
  scriptEngine->setObjectToScript(type,
                                _T("void SetTransform(Transform@ trans)"),
                                0,
                                &m_gameObject.getTransform(),
                                obj);
                                
  scriptEngine->executeFunctionParam(_T("void onKeyUp(KeyCode)"),
                                     type,
                                     obj,
                                     key);
}

void
ScriptComponent::setScriptLocalProperties() {
  Int8 result;

  
}

}

