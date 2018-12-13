#include "dr_scrip_inputs.h"
#include <dr_script_component.h>
#include <dr_script_core.h>
#include <dr_resource_manager.h>
namespace driderSDK {

void
ScripInputs::getInputs(TString * response) {
  auto& scriptComponent = static_cast<ScriptComponent&>(m_component);
  auto script = scriptComponent.getScript();
  TString name;
  if (scriptComponent.isEmpty())
  {
    name = _T("");
  }
  else
  {
    name = scriptComponent.m_script->getName();
  }
  (*response) += addInput(_T("0"), _T("droppableFile"), _T("Script"), name);

}

bool
ScripInputs::changeValue(TString &value, TString &id) {
  auto& scriptComponent = static_cast<ScriptComponent&>(m_component);
  if (id == _T("0"))
  {
    auto ptr = ResourceManager::loadResource(value);
    auto ptrScript = std::dynamic_pointer_cast<ScriptCore>(ptr);
    scriptComponent.setScript(ptrScript);
    scriptComponent.initScript();
    scriptComponent.getScriptEngine()->m_scriptEngine->GetModule("GameModule")->Build();
     // auto currentModule = scriptEngine->m_scriptEngine->GetModule("GameModule");
    //result = currentModule->Build();
  }
  return false;
}

}
