#include "dr_camera_inputs.h"
#include <dr_camera_component.h>

namespace driderSDK {

void
CameraInputs::getInputs(TString * response) {

  auto camera = static_cast<CameraComponent&>(m_component);

  (*response) += addInput(_T("0"), _T("numberFloat"), _T("nearPlane "), StringUtils::toTString(int(camera.getNearPlane())));
  (*response) += addInput(_T("1"), _T("number"), _T("farPlane "), StringUtils::toTString(int(camera.getFarPlane())));
  //camera.setActive();
  std::vector<TString> options = { _T("Ortographic"),
                                   _T("Perspective")};
  (*response) += addInputSelectable(_T("2"),
                                     _T("Type"), 
                                     StringUtils::toTString((Int32)camera.getTypeCamera()),
                                     &options);
  if (camera.getTypeCamera() == CameraViewType::kPerspective)
  {
    (*response) += addInput(_T("3"), _T("number"), _T("fov  "), StringUtils::toTString(camera.getFov(), 2));

  }
  else {
    (*response) += addInput(_T("4"), _T("number"), _T("width  "), StringUtils::toTString(camera.getWidth(), 2));
    (*response) += addInput(_T("5"), _T("number"), _T("height  "), StringUtils::toTString(camera.getHeight(), 2));
  }



  //camera.getFarPlane();
  //camera.getNearPlane();
}

bool
CameraInputs::changeValue(TString &value, TString &id) {
  auto camera = static_cast<CameraComponent&>(m_component);
  if (id == _T("0"))
  {
    camera.setNearPlane(StringUtils::toReal(value));
  }
  else if (id == _T("1"))
  {
    camera.setFarPlane(StringUtils::toReal(value));
  }
  else if (id == _T("2"))
  {
    camera.setTypeCamera((CameraViewType::E)StringUtils::toInt(value));
    return true;
  }
  else if (id == _T("3"))
  {
    camera.setFov(StringUtils::toReal(value));
  }
  else if (id == _T("4"))
  {
    camera.setWidth(StringUtils::toReal(value));
  }
  else if (id == _T("5"))
  {
    camera.setHeight(StringUtils::toReal(value));
  }
  return false;
}

}
