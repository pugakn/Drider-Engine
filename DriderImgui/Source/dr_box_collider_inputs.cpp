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
    float w = aabb.width;
    if(ImGui::InputFloat("Width:", &w) && w > 0.0f) {
      aabb.width = w;
      boxComponent.setAABB(aabb);
    }

    float h = aabb.height;
    if (ImGui::InputFloat("Height:", &h) && h > 0.0f) {
      aabb.height = h;
      boxComponent.setAABB(aabb);
    }

    float d = aabb.depth;
    if (ImGui::InputFloat("Depth:", &d) && d > 0.0f) {
      aabb.depth = d;
      boxComponent.setAABB(aabb);
    }

    Vector3D center = aabb.center;
    if(ImGui::InputFloat3("Center:", center.ptr())) {
      aabb.center = center;
      boxComponent.setAABB(aabb);
    }
    
  }
}

}
