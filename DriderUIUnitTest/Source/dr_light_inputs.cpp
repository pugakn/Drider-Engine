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
  Vector3D lightColor = light.GetColor();

  (*response) += addInput(_T("0"), _T("number"), _T("Range"), StringUtils::toTString(light.GetRange(), 4));
  (*response) += addInput(_T("1"), _T("number"), _T("Intensity"), StringUtils::toTString(light.GetIntensity(), 4));
  (*response) += addInput(_T("2"), _T("number"), _T("R"), StringUtils::toTString(Math::floor(lightColor.x * 255.f), 4));
  (*response) += addInput(_T("3"), _T("number"), _T("G"), StringUtils::toTString(Math::floor(lightColor.x * 255.f), 4));
  (*response) += addInput(_T("4"), _T("number"), _T("B"), StringUtils::toTString(Math::floor(lightColor.x * 255.f), 4));


//#ifdef UNICODE
//  std::wstringstream hexValue;
//#else
//  std::stringstream hexValue;
//#endif // UNICODE
//  UInt32 temp = 0;
//  temp = static_cast<UInt32>(Math::floor(lightColor.x * 255.f)) << 16 |
//         static_cast<UInt32>(Math::floor(lightColor.y * 255.f)) << 8 |
//         static_cast<UInt32>(Math::floor(lightColor.z * 255.f));
//  hexValue << _T("#") << std::setfill(L'0') << std::setw(6) << std::hex << temp;
//  auto temporal = hexValue.str();
//  (*response) += addInput(_T("2"), _T("color"), _T("Color"), hexValue.str());
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
  auto& ligth = static_cast<LightComponent&>(m_component);
  if (id == _T("0"))
  {
    ligth.SetRange(StringUtils::toReal(value));
  }
  else if (id == _T("1"))
  {
    ligth.SetIntensity(StringUtils::toReal(value));
  }
  else
  {
    Vector3D lightColor = ligth.GetColor();
    float valueLigth = StringUtils::toReal(value) / 255.f;
    if (id == _T("2"))
    {
      ligth.SetColor({ valueLigth, lightColor.y, lightColor.z });
    }
    else if (id == _T("3"))
    {
      ligth.SetColor({ lightColor.x, valueLigth, lightColor.z });
    }
    else if (id == _T("4"))
    {
      ligth.SetColor({ lightColor.x, lightColor.y, valueLigth });
    }
  }

  return false;
}

}
