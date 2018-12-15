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

ScriptComponent::ScriptComponent(GameObject &gameObject) :
  GameComponent(gameObject, _T("ScriptComponent")) {

  scriptEngine = ScriptEngine::instancePtr();

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
                              std::bind(&ScriptComponent::onKeyDown, this, std::placeholders::_1));

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyReleased,
                              std::bind(&ScriptComponent::onKeyUp, this, std::placeholders::_1));

  //Get module
  mod = scriptEngine->m_scriptEngine->GetModule("GameModule");

}

ScriptComponent::~ScriptComponent() {

}

void
ScriptComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Script;
  file.m_file << StringUtils::toString(getName()) << "\n";

}

void
ScriptComponent::deserialize(TString &data) {
  //Not implemented
}

void
ScriptComponent::onCreate() {
  
}

void
ScriptComponent::onUpdate() {
  if (isEmpty())
  {
    return;
  }
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

UInt32 ScriptComponent::getClassID()
{
  return CLASS_NAME_ID(ScriptComponent);
}

/**
* Clones the component inside the given gameObject
*/
GameComponent*
ScriptComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<ScriptComponent>(m_script);
}

void
ScriptComponent::initScript() {
  //Get script object
  TString name = m_script->getName();

  scriptEngine->getScriptObject(name,
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

asIScriptObject*
ScriptComponent::getScript() {
  if(obj == 0)
    return 0;
  
  obj->AddRef();
  return obj;
}

void
ScriptComponent::setScript(std::shared_ptr<ScriptCore> _script) {
  m_script = _script;

  scriptEngine->addScript(m_script->getName(),
                          m_script->getScript(),
                          _T("GameModule"));
}

}

