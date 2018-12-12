#include "dr_render_inputs.h"
#include <dr_memory.h>
#include <dr_render_component.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <vector>
namespace driderSDK {

void
RenderInputs::getInputs(TString * response) {
  auto& render = static_cast<RenderComponent&>(m_component);
  auto model = render.getModel().lock();
  if (model)
  {
    (*response) += addInput(_T("0"), _T("droppableFile"), _T("Model"), model->getName());
  }
  else
  {
    (*response) += addInput(_T("0"), _T("droppableFile"), _T("Model"), _T(""));
  }
  /*
  (*response) += addInput(_T("1"), _T("number"), _T("altura"), _T("0"));
  (*response) += addInput(_T("2"), _T("checkbox"), _T("activado"), _T("true"));
  (*response) += addInput(_T("3"), _T("checkbox"), _T("activado"), _T("false"));
  (*response) += addInput(_T("4"), _T("color"), _T("color"), _T("#ff0000"));
  (*response) += addInput(_T("5"), _T("droppableFile"), _T("Albedo"), _T("colo.x"));
  (*response) += addInput(_T("5"), _T("droppableGO"), _T("colider"), _T("piso"));

  */
  std::vector<TString> options = { _T("text"),
                                   _T("number"),
                                   _T("checkbox"),
                                   _T("color"),
                                   _T("droppableFile"),
                                   _T("droppableGO")};
  (*response) += addInputSelectable(_T("5"),
                                    _T("Tipos de campos"),
                                    _T("4"),
                                    &options);
  
}

bool
RenderInputs::changeValue(TString &value, TString &id) {

  auto& render = static_cast<RenderComponent&>(m_component);
  auto ptr = ResourceManager::loadResource(value);
  auto ptrModel = std::dynamic_pointer_cast<Model>(ptr);

  render.setModel(ptrModel);
  return false;
}

}
