#include "dr_script_inputs.h"

#include "dr_script_component.h"
#include "dr_script_core.h"
#include "dr_resource_manager.h"
#include "imgui_stdlib.h"

namespace driderSDK {

void
ScriptInputs::getInputs() {
  auto& script = static_cast<ScriptComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(script.getName()).c_str())) {
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
  }
}

}