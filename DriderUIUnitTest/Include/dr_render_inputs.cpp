#include "dr_render_inputs.h"
#include <vector>

void
driderSDK::RenderInputs::getInputs(TString * response) {
  (*response) += addInput(_T("0"), _T("text"), _T("nombre"), _T("lucas"));
  (*response) += addInput(_T("1"), _T("number"), _T("altura"), _T("0"));
  (*response) += addInput(_T("2"), _T("checkbox"), _T("activado"), _T("true"));
  (*response) += addInput(_T("3"), _T("checkbox"), _T("activado"), _T("false"));
  (*response) += addInput(_T("4"), _T("color"), _T("color"), _T("#ff0000"));
  (*response) += addInput(_T("5"), _T("droppableFile"), _T("Albedo"), _T("colo.x"));
  (*response) += addInput(_T("5"), _T("droppableGO"), _T("colider"), _T("piso"));

  std::vector<TString> options = { _T("text"),
                                   _T("number"),
                                   _T("checkbox"),
                                   _T("color"),
                                   _T("droppableFile"),
                                   _T("droppableGO")};
  (*response) += addInputSelectable(_T("5"),
                                    _T("Tipos de campos"),
                                    _T("colider"),
                                    &options);
  

}
