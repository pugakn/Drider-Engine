#include "dr_animator_inputs.h"
#include <dr_animator_component.h>
namespace driderSDK {

void
AnimatorInputs::getInputs() {
  auto& animatorComponent = static_cast<AnimatorComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(animatorComponent.getName()).c_str())) {
  }
}

}
