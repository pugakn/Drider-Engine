#include "dr_light_inputs.h"

#include <dr_point_light_component.h>
#include <dr_Directional_light_component.h>

namespace driderSDK {

void
PointLightInputs::getInputs() {
  auto& lightComponent = static_cast<PointLightComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(lightComponent.getName()).c_str())) {
    ImGui::Text("Range:");
    ImGui::SameLine();
    auto tempRange = lightComponent.GetRange();
    if (ImGui::DragFloat("##rangeLightComponent", &tempRange)) {
      lightComponent.SetRange(tempRange);
    }
    ImGui::Text("Intensity:");
    ImGui::SameLine();
    auto tempIntensity = lightComponent.GetIntensity();
    if (ImGui::DragFloat("##intensityLightComponent", &tempIntensity)) {
      lightComponent.SetIntensity(tempIntensity);
    }
    
    ImGui::Text("Color:");
    Vector3D tempcolor = lightComponent.GetColor();
    if (ImGui::ColorEdit3("Color##colorLightComponent", (float*)&tempcolor, ImGuiColorEditFlags_Float))
    {
      lightComponent.SetColor(tempcolor);
    }

  }
}

void
DirectionalLightInputs::getInputs() {
  auto& lightComponent = static_cast<DirectionalLightComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(lightComponent.getName()).c_str())) {
    ImGui::Text("Cast shadow:");
    ImGui::SameLine();
    auto tempShadow = lightComponent.GetCastShadow();
    if (ImGui::Checkbox("##rangeLightComponent", &tempShadow)) {
      lightComponent.SetCastShadow(tempShadow);
    }

    ImGui::Text("Intensity:");
    ImGui::SameLine();
    auto tempIntensity = lightComponent.GetIntensity();
    if (ImGui::DragFloat("##intensityLightComponent", &tempIntensity)) {
      lightComponent.SetIntensity(tempIntensity);
    }
    
    ImGui::Text("Color:");
    Vector3D tempcolor = lightComponent.GetColor();
    if (ImGui::ColorEdit3("color##colorLightComponent", (float*)&tempcolor, ImGuiColorEditFlags_Float))
    {
      lightComponent.SetColor(tempcolor);
    }

  }
}

}