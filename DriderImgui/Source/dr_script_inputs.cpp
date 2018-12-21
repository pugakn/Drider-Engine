#include "dr_script_inputs.h"

#include "dr_script_component.h"
#include "dr_script_core.h"
#include "dr_resource_manager.h"
#include "imgui_stdlib.h"

namespace driderSDK {

void
ScriptInputs::getInputs() {
  auto& script = static_cast<ScriptComponent&>(m_component);
  TString temp = _T("");
  if (script.m_script != nullptr) {
    temp = script.m_script->getName();
  }
  ImGui::Text("Name:"); ImGui::SameLine();
  ImGui::InputText("##script", &temp, ImGuiInputTextFlags_ReadOnly);
  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_ITEM"))
    {
      char* data = (char*)payload->Data;
      auto ptr = ResourceManager::loadResource(StringUtils::toTString(data));
      auto ptrScript = std::dynamic_pointer_cast<ScriptCore>(ptr);

      script.setScript(ptrScript);
      
      //Build module
      /*auto currentModule = ScriptEngine::instance().m_scriptEngine->GetModule("GameModule");
      Int32 result = currentModule->Build();

      script.initScript();
      script.start();*/
    }
    ImGui::EndDragDropTarget();
  }

  if(script.isInitilized()) {
    Int32 numProperties = script.getObject()->GetPropertyCount();
    for(Int32 i = 2; i < numProperties; i++) {
      auto var = script.getObject()->GetAddressOfProperty(i);
      auto type = script.getObject()->GetPropertyTypeId(i);
      auto name = script.getObject()->GetPropertyName(i);
      
      loadVar(var, type, name);
    }
  }
}

void
ScriptInputs::loadVar(void* dir,
                      Int32 type,
                      const char* name) {
  switch (type)
  {
  case driderSDK::VAR_TYPES::VOID: {
    ImGui::Text(name); ImGui::SameLine();
    ImGui::TextDisabled("void");
  }
    break;
  case driderSDK::VAR_TYPES::INT8:
  case driderSDK::VAR_TYPES::INT16:
  case driderSDK::VAR_TYPES::INT:
  case driderSDK::VAR_TYPES::INT64:
  case driderSDK::VAR_TYPES::UINT8:
  case driderSDK::VAR_TYPES::UINT16:
  case driderSDK::VAR_TYPES::UINT:
  case driderSDK::VAR_TYPES::UINT64: {
    ImGui::Text(name); ImGui::SameLine();
    Int32* var = reinterpret_cast<Int32*>(dir);
    if(ImGui::InputInt(String("##" + StringUtils::toString(name)).c_str(), var)) {
      dir = &var;
    }
  }
    break;
  case driderSDK::VAR_TYPES::FLOAT: {
    ImGui::Text(name); ImGui::SameLine();
    float* var2 = reinterpret_cast<float*>(dir);
    if (ImGui::InputFloat(String("##" + StringUtils::toString(name)).c_str(), var2)) {
      dir = &var2;
    }
  }
    break;
  case driderSDK::VAR_TYPES::DOUBLE: {
    ImGui::Text(name); ImGui::SameLine();
    double* var2 = reinterpret_cast<double*>(dir);
    if (ImGui::InputDouble(String("##" + StringUtils::toString(name)).c_str(), var2)) {
      dir = &var2;
    }
  }
    break;
  case driderSDK::VAR_TYPES::BOOL: {
    ImGui::Text(name); ImGui::SameLine();
    bool* var3 = reinterpret_cast<bool*>(dir);
    if (ImGui::Checkbox(String("##" + StringUtils::toString(name)).c_str(), var3)) {
      dir = &var3;
    }
  }
    break;
  default:
    break;
  }

}

}