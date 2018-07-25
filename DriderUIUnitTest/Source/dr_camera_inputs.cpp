#include "..\Include\dr_camera_inputs.h"
#include <dr_camera_component.h>

namespace driderSDK {

void
CameraInputs::getInputs(TString * response) {

  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  
  auto render = static_cast<CameraComponent&>(m_component);

}

void
CameraInputs::changeValue(TString &value, TString &id) {
}

}
