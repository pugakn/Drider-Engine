#include "..\Include\dr_sound_inputs.h"
#include <dr_sound_component.h>

namespace driderSDK {

void
SoundInputs::getInputs(TString * response) {

  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  
  auto render = static_cast<SoundComponent&>(m_component);

}

void
SoundInputs::changeValue(TString &value, TString &id) {
}

}
