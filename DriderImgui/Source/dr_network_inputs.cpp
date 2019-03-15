#include "dr_network_inputs.h"

#include <dr_networkManager_component.h>

namespace driderSDK {

void
NetworkManagerInput::getInputs() {
  auto& NetComponent = static_cast<NetworkManagerComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(NetComponent.getName()).c_str())) {
    ImGui::Text("Info no disponible");
  }
}

}