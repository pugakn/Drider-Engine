#include "..\Include\dr_input_editor.h"

#include <unordered_map>
#include <functional>

#include <dr_id_object.h>
#include <dr_gameComponent.h>

namespace driderSDK {

InputEditor::InputEditor(GameComponent& _component) : m_component(_component)
{}

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
  
  using Factory = std::function<std::unique_ptr<InputEditor>()>;
  static std::unordered_map<UInt32, Factory> m_factories;
 
  m_factories[CLASS_NAME_ID(RenderComponent)] = dr_make_unique<RenderInputs>;
  //m_factories[CLASS_NAME_ID(RenderComponent)] = dr_make_unique<RenderInputs>;
  
  return m_factories[_component->getClassID()]();
}

}