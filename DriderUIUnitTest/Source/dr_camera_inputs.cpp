#include "dr_camera_inputs.h"
#include <dr_camera_component.h>

namespace driderSDK {

void
CameraInputs::getInputs(TString * response) {

  auto camera = static_cast<CameraComponent&>(m_component);

  (*response) += addInput(_T("0"), _T("number"), _T("nearPlane "), StringUtils::toTString(int(camera.getNearPlane())));
  (*response) += addInput(_T("1"), _T("number"), _T("farPlane "), StringUtils::toTString(int(camera.getFarPlane())));
  std::vector<TString> options = { _T("Perspective"),
                                   _T("Ortographic")};
  (*response) += addInputSelectable(_T("2"),
                                     _T("Type"),
                                     _T("4"),
                                     &options);
  if (camera.getFarPlane() > 10)
  {
    (*response) += addInput(_T("3"), _T("number"), _T("fov  "), _T("120"));

  }
  else {
    (*response) += addInput(_T("4"), _T("number"), _T("width  "), _T("130"));
    (*response) += addInput(_T("5"), _T("number"), _T("height  "), _T("140"));
  }



  //camera.getFarPlane();
  //camera.getNearPlane();
}

void
CameraInputs::changeValue(TString &value, TString &id) {
  auto camera = static_cast<CameraComponent&>(m_component);
  if (id == _T("1"))
  {
    camera.setFarPlane(StringUtils::toReal(value));

  }
}

}
