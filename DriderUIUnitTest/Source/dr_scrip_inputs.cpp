#include "dr_scrip_inputs.h"
#include <dr_script_component.h>

namespace driderSDK {

void
ScripInputs::getInputs(TString * response) {

  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  
  auto render = static_cast<ScriptComponent&>(m_component);

}

void
ScripInputs::changeValue(TString &value, TString &id) {
}

}
