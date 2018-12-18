#include "dr_render_inputs.h"
#include <dr_model.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>

#include "imgui_stdlib.h"
namespace driderSDK {

void
RenderInputs::getInputs() {
  auto& render = static_cast<RenderComponent&>(m_component);
  auto model = render.getModel().lock();
  TString temp = _T("");
  if (model) {
    temp = model->getName();
  }
  ImGui::Text("Name:"); ImGui::SameLine();
  ImGui::InputText("##modelRenderInputs", &temp, ImGuiInputTextFlags_ReadOnly);
  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_ITEM"))
    {
      char* lu = (char*)payload->Data;
      auto& render = static_cast<RenderComponent&>(m_component);
      auto ptr = ResourceManager::loadResource(StringUtils::toTString(lu));
      auto ptrModel = std::dynamic_pointer_cast<Model>(ptr);

      render.setModel(ptrModel);
    }
    ImGui::EndDragDropTarget();
  }
}

}