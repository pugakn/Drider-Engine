#include "dr_input_editor.h"

#include <unordered_map>
#include <functional>

#include <dr_id_object.h>
#include <dr_aabb_collider.h>
#include <dr_animator_component.h>
#include <dr_camera_component.h>
#include <dr_script_component.h>
#include <dr_sound_component.h>
#include <dr_render_component.h>
#include <dr_gameComponent.h>

#include "dr_aabb_collider_inputs.h"
#include "dr_animator_inputs.h"
#include "dr_camera_inputs.h"
#include "dr_render_inputs.h"
#include "dr_scrip_inputs.h"
#include "dr_sound_inputs.h"



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
InputEditor::createInputEditor(GameComponent &_component) {
  using UniqueInput = std::unique_ptr<InputEditor>;
  using Factory = std::function<UniqueInput(GameComponent&)>;

  static std::unordered_map<UInt32, Factory> m_factories
  { 
    //Template
    //{CLASS_NAME_ID(Class), dr_make_unique<Class, GameComponent&>}
    {CLASS_NAME_ID(AABBCollider), dr_make_unique<AABBColliderInputs, GameComponent&>},
    {CLASS_NAME_ID(AnimatorComponent), dr_make_unique<AnimatorInputs, GameComponent&>},
    {CLASS_NAME_ID(RenderComponent), dr_make_unique<RenderInputs, GameComponent&>},
    {CLASS_NAME_ID(ScriptComponent), dr_make_unique<ScripInputs, GameComponent&>},
    {CLASS_NAME_ID(SoundComponent), dr_make_unique<SoundInputs, GameComponent&>},
    {CLASS_NAME_ID(CameraComponent), dr_make_unique<CameraInputs, GameComponent&>}
  };

  std::unordered_map<UInt32, Factory>::const_iterator got = m_factories.find(_component.getClassID());

  if (got == m_factories.end()) {
    m_factories.insert({ _component.getClassID(),dr_make_unique<CameraInputs, GameComponent&>});
  }

  UInt32 temp = _component.getClassID();
  UInt32 temp2 = CLASS_NAME_ID(Camera);
  return m_factories[_component.getClassID()](_component);
}

}