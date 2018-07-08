#include "..\Include\dr_input_editor.h"

#include <unordered_map>
#include <functional>

#include <dr_id_object.h>
#include <dr_gameComponent.h>

#include "..\Include\dr_render_inputs.h"

namespace driderSDK {

InputEditor::InputEditor(GameComponent& _component) : m_component(_component)
{}

InputEditor::~InputEditor() {}

TString
driderSDK::InputEditor::addInput(TString id,
                                 TString type,
                                 TString name,
                                 TString value) {
  TString response = _T("{");
  response += _T("'type': '") + type + _T("',");
  response += _T("'name': '") + name + _T("',");
  response += _T("'id': '") + id + _T("',");
  response += _T("'value': '") + value + _T("'},");
  return response;
}

TString
driderSDK::InputEditor::addInputSelectable(TString id,
                                           TString name,
                                           TString selected,
                                           std::vector<TString>* options)
{
  TString response = _T("{");
  response += _T("'type': 'selectable',");
  response += _T("'name': '") + name + _T("',");
  response += _T("'id': '") + id + _T("',");
  response += _T("'selected': '") + selected + _T("',");
  response += _T("'options': [");

  for (size_t i = 0; i < options->size(); i++)
  {
    response += _T("{'name': '") + (*options)[i] + _T("'},");
  }
  response.erase(response.length() - 1);
  response += _T("]},");

  return response;
}

std::unique_ptr<InputEditor> 
InputEditor::createInputEditor(GameComponent& _component) {
  
  using UniqueInput = std::unique_ptr<InputEditor>;
  using Factory = std::function<UniqueInput(GameComponent&)>;

  static std::unordered_map<UInt32, Factory> m_factories
  { 
    //Template
    //{CLASS_NAME_ID(Class), dr_make_unique<Class, GameComponent&>}
    {CLASS_NAME_ID(RenderComponent), dr_make_unique<RenderInputs, GameComponent&>}
  };
 
  //m_factories[CLASS_NAME_ID(RenderComponent)] = dr_make_unique<RenderInputs>;
  
  return m_factories[_component.getClassID()](_component);
}

}