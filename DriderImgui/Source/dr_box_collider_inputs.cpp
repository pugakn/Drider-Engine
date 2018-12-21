#include "dr_box_collider_inputs.h"

#include <dr_box_collider.h>
#include <dr_collision_body.h>

namespace driderSDK {

void
BoxColliderInputs::getInputs() {
  auto& boxComponent = static_cast<BoxCollider&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(boxComponent.getName()).c_str())) {

    //AABB
    ImGui::TextDisabled("Box(1.0, 1.0, 1.0)");
    
  }
}

}
