#include "dr_light_inputs.h"
#include <dr_memory.h>
#include <dr_light_component.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <vector>
#include <sstream>
#include <iomanip>
namespace driderSDK {

void
LightInputs::getInputs(TString * response) {
  auto& light = static_cast<LightComponent&>(m_component);
  (*response) += addInput(_T("0"), _T("number"), _T("Rango"), StringUtils::toTString(light.GetRange(), 4));
  (*response) += addInput(_T("1"), _T("number"), _T("Intensidad"), StringUtils::toTString(light.GetIntensity(), 4));
  Vector3D lightColor = light.GetColor();
#ifdef UNICODE
  std::wstringstream hexValue;
#else
  std::stringstream hexValue;
#endif // UNICODE
  UInt32 temp = 0;
  temp = static_cast<UInt32>(Math::floor(0.f * 255.f)) << 16 | 
         static_cast<UInt32>(Math::floor(.2f * 255.f)) << 8 |
         static_cast<UInt32>(Math::floor(lightColor.z * 255.f));
  hexValue << _T("#") << std::setfill(L'0') << std::setw(6) << std::hex << temp;
  auto temporal = hexValue.str();
  (*response) += addInput(_T("2"), _T("color"), _T("Color"), hexValue.str());
  /*
  (*response) += addInput(_T("1"), _T("number"), _T("altura"), _T("0"));
  (*response) += addInput(_T("2"), _T("checkbox"), _T("activado"), _T("true"));
  (*response) += addInput(_T("3"), _T("checkbox"), _T("activado"), _T("false"));
  (*response) += addInput(_T("4"), _T("color"), _T("color"), _T("#ff0000"));
  (*response) += addInput(_T("5"), _T("droppableFile"), _T("Albedo"), _T("colo.x"));
  (*response) += addInput(_T("5"), _T("droppableGO"), _T("colider"), _T("piso"));

  */
}

bool
LightInputs::changeValue(TString &value, TString &id) {

  auto& render = static_cast<LightComponent&>(m_component);

  return false;
}

}
