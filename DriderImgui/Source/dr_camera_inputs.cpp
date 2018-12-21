#include "dr_camera_inputs.h"
#include <dr_camera_component.h>
namespace driderSDK {

void
CameraInputs::getInputs() {
  auto& cameraComponent = static_cast<CameraComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(cameraComponent.getName()).c_str())) {
  }
}

}
