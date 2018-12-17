#include "dr_light_inputs.h"

#include <dr_light_component.h>

namespace driderSDK {

void
LightInputs::getInputs() {
  auto& lightComponent = static_cast<LightComponent&>(m_component);
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
    
    ImGui::Text("Color widget with Float Display:");
    Vector3D tempcolor = lightComponent.GetColor();
    if (ImGui::ColorEdit3("MyColor##colorLightComponent", (float*)&tempcolor, ImGuiColorEditFlags_Float))
    {
      lightComponent.SetColor(tempcolor);
    }

  }
}

}