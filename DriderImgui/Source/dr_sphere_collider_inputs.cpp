#include "dr_sphere_collider_inputs.h"

#include <dr_sphere_collider.h>

namespace driderSDK {

void
SphereColliderInputs::getInputs() {
  auto& sphereComponent = static_cast<SphereCollider&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(sphereComponent.getName()).c_str())) {

    //AABB
    ImGui::TextDisabled("Sphere(1.0, (0.0, 0.0, 0.0) )");

  }
}

}