#include "..\Include\dr_animator_inputs.h"
#include <dr_animator_component.h>

namespace driderSDK {

void
AnimatorInputs::getInputs(TString * response) {

  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  
  auto render = static_cast<AnimatorComponent&>(m_component);

  

}

void
AnimatorInputs::changeValue(TString &value, TString &id) {
}

}
