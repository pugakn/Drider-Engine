#include "dr_aabb_collider_inputs.h"
#include <dr_aabb_collider.h>

namespace driderSDK {

void
AABBColliderInputs::getInputs(TString * response) {

  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  
  auto& render = static_cast<AABBCollider&>(m_component);

  

}

void
AABBColliderInputs::changeValue(TString &value, TString &id) {
}

}
