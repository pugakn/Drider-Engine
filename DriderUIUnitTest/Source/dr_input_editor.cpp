#include "..\Include\dr_input_editor.h"

namespace driderSDK {

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

}