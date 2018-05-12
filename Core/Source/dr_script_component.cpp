#include "dr_script_component.h"
#include <functional>

#include <..\..\Script\Include\dr_context_manager.h>
#include <..\..\Script\Include\dr_script_engine.h>

#include "dr_script_core.h"
#include <dr_gameObject.h>

#include <dr_graph.h>

namespace driderSDK {


ScriptComponent::ScriptComponent(GameObject &gameObject,
                                 std::shared_ptr<ScriptCore> _script) 
  : GameComponent(gameObject, _T("ScriptComponent")),
    m_script(_script) {
  scriptEngine = ScriptEngine::instancePtr();

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
                              std::bind(&ScriptComponent::onKeyDown, this, std::placeholders::_1));

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyReleased,
                              std::bind(&ScriptComponent::onKeyUp, this, std::placeholders::_1));

  //Add script section
  scriptEngine->addScript(m_script->getName(),
                          m_script->getScript(),
                          _T("GameModule"));

  //Get module
  mod = scriptEngine->m_scriptEngine->GetModule("GameModule");

  //Compile the module after adding a new script
  //Int8 result = mod->Build();
}

ScriptComponent::~ScriptComponent() {

}

void
ScriptComponent::onCreate() {
  
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

/**
* Clones the component inside the given gameObject
*/
void
ScriptComponent::cloneIn(GameObject& _go) {

}

void
ScriptComponent::initScript() {
  //Get script object
  scriptEngine->getScriptObject(m_script->getName(),
                                mod,
                                &obj,
                                &type);
}

void
ScriptComponent::start() {
  scriptEngine->setObjectToScript(type,
                                  _T("void SetTransform(Transform@ trans)"),
                                  0,
                                  &m_gameObject.getTransform(),
                                  obj);

  scriptEngine->setObjectToScript(type,
                                  _T("void SetGameObject(GameObject@ object)"),
                                  0,
                                  &SceneGraph::getRoot(),
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

