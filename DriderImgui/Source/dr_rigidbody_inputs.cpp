#include "dr_rigidbody_inputs.h"

#include <dr_rigidbody_component.h>
#include <dr_rigidbody.h>

namespace driderSDK {

void
RigidbodyInputs::getInputs() {
  auto& rigidBodyComponent = static_cast<RigidBody3DComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(rigidBodyComponent.getName()).c_str())) {

    // Gravity
    static bool tempGravity = false;
    //rigidBodyComponent.enableGravity(false);
    if(ImGui::Checkbox("Use gravity", &tempGravity)) {
      rigidBodyComponent.enableGravity(tempGravity);
      //rigidBodyComponent.setType(RIGID_BODY_TYPE::kStatic);
    }
    /*ImGui::SameLine(); 
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Activate the gravity");
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }*/

    // Type
    static Int32 type = 0;
    if(ImGui::Combo("Inputs Mode", &type, "Static\0Kinematic\0Dynamic\0")) {
      rigidBodyComponent.setType((RIGID_BODY_TYPE::E)type);
    }

  }
}

}