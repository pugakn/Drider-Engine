#include "dr_render_inputs.h"
#include <dr_model.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>

#include "imgui_stdlib.h"
namespace driderSDK {

void
RenderInputs::getInputs() {
  auto& render = static_cast<RenderComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(render.getName()).c_str())) {
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
    /*SizeT meshes = model->meshes.size();
    ImGui::Text("Meshes: %i", meshes);*/
    Int32 counter = 0;
    for(auto &mesh : render.getMeshes()) {
      TString name = _T("");
      if(mesh.material.lock()) {
        name = mesh.material.lock()->getName();
      }
      ImGui::Text((std::string("Material") + StringUtils::toString(counter)).c_str()); ImGui::SameLine();

      ImGui::InputText((std::string("##material") + StringUtils::toString(counter)).c_str(), &name, ImGuiInputTextFlags_ReadOnly);
      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_ITEM"))
        {
          char* data = (char*)payload->Data;
          auto ptr = ResourceManager::loadResource(StringUtils::toTString(data));
          auto ptrMat = std::dynamic_pointer_cast<Material>(ptr);

          mesh.material = ptrMat;
        }
        ImGui::EndDragDropTarget();
      }
      ++counter;
    }
  }
}
}