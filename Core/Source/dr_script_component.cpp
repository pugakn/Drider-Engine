#include "dr_script_component.h"
#include <functional>

#include <..\..\Script\Include\dr_context_manager.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

ScriptComponent::~ScriptComponent() {

}

void
ScriptComponent::onCreate() {
  
  scriptEngine = ScriptEngine::instancePtr();

  if (!ContextManager::isStarted()) {
    ContextManager::startUp();
  }
  ctxMag = ContextManager::instancePtr();

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
                              std::bind(&ScriptComponent::onKeyDown, this, std::placeholders::_1));
}

void
ScriptComponent::onUpdate() {
  /*ctxMag->addContext(scriptEngine->m_scriptEngine,
                     scriptEngine->m_scriptEngine->GetModule(
                     StringUtils::toString(m_module).c_str())->GetFunctionByDecl(
                     "void Update()"));

  Int8 result = ctxMag->executeScripts();*/
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
  
  m_scriptName = name;
  m_script = script;
  m_module = module;

}

/**
* Clones the component inside the given gameObject
*/
void
ScriptComponent::cloneIn(GameObject& _go) {

}

void
ScriptComponent::start() {
  //Add context
  /*ctxMag->addContext(scriptEngine->m_scriptEngine,
                     scriptEngine->m_scriptEngine->GetModule(
                     StringUtils::toString(m_module).c_str())->GetFunctionByDecl(
                     "void Start()"));

  Int8 result = ctxMag->executeScripts();*/
}

void
ScriptComponent::onKeyDown(KEY_CODE::E key) {
  /*asIScriptContext *ctx = ctxMag->addContext(scriptEngine->m_scriptEngine,
                                             scriptEngine->m_scriptEngine->GetModule(
                                             StringUtils::toString(m_module).c_str())->GetFunctionByDecl(
                                               "void onKeyDown(KeyCode)"));
  if(ctx != 0) {
    Int8 result;
    result = ctx->SetArgDWord(0, (Int32)key);
    result = ctxMag->executeScripts();
  }*/
}


void
ScriptComponent::compileScript() {
  scriptEngine->compile(m_module);
}

void
ScriptComponent::setScriptLocalProperties() {
  Int8 result;

  
}

}

