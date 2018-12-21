#include "dr_box_collider_inputs.h"

#include <dr_box_collider.h>
#include <dr_collision_body.h>

namespace driderSDK {

void
BoxColliderInputs::getInputs() {
  auto& boxComponent = static_cast<BoxCollider&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(boxComponent.getName()).c_str())) {

    //AABB
    //ImGui::TextDisabled("Box(1.0, 1.0, 1.0)");
    AABB aabb = boxComponent.getAABB();

  /*	float width, height, depth;

	Vector3D center;*/
    if(ImGui::InputFloat("Width:", &aabb.width)) {
      boxComponent.setAABB(aabb);
    }

    if (ImGui::InputFloat("Height:", &aabb.height)) {
      boxComponent.setAABB(aabb);
    }

    if (ImGui::InputFloat("Depth:", &aabb.depth)) {
      boxComponent.setAABB(aabb);
    }

    if(ImGui::InputFloat3("Center:", aabb.center.ptr())) {
      boxComponent.setAABB(aabb);
    }
    
  }
}

}
