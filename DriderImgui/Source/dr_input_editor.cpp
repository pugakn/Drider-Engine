#include "dr_input_editor.h"

#include <unordered_map>
#include <functional>

#include "dr_animator_inputs.h"
#include "dr_box_collider_inputs.h"
#include "dr_camera_inputs.h"
#include "dr_light_inputs.h"
#include "dr_render_inputs.h"
#include "dr_rigidbody_inputs.h"
#include "dr_sphere_collider_inputs.h"
#include "dr_script_inputs.h"
#include "dr_sound_inputs.h"

#include <dr_animator_component.h>
#include <dr_box_collider.h>
#include <dr_camera_component.h>
#include <dr_gameComponent.h>
#include <dr_id_object.h>
#include <dr_light_component.h>
#include <dr_render_component.h>
#include <dr_rigidbody_component.h>
#include <dr_sphere_collider.h>
#include <dr_script_component.h>
#include <dr_sound_component.h>

//#include "dr_render_inputs.h"


namespace driderSDK {

  InputEditor::InputEditor(GameComponent& _component) : m_component(_component)
  {}

  InputEditor::~InputEditor() {}

  std::unique_ptr<InputEditor>
    InputEditor::createInputEditor(GameComponent &_component) {
    using UniqueInput = std::unique_ptr<InputEditor>;
    using Factory = std::function<UniqueInput(GameComponent&)>;

    static std::unordered_map<UInt32, Factory> m_factories
    {
      //Template
      //{CLASS_NAME_ID(Class), dr_make_unique<Class, GameComponent&>}
      {CLASS_NAME_ID(AnimatorComponent), dr_make_unique<AnimatorInputs, GameComponent&>},
      {CLASS_NAME_ID(BoxCollider), dr_make_unique<BoxColliderInputs, GameComponent&>},
      {CLASS_NAME_ID(CameraComponent), dr_make_unique<CameraInputs, GameComponent&>},
      {CLASS_NAME_ID(LightComponent), dr_make_unique<LightInputs, GameComponent&>},
      {CLASS_NAME_ID(RenderComponent), dr_make_unique<RenderInputs, GameComponent&>},
      {CLASS_NAME_ID(RigidBody3DComponent), dr_make_unique<RigidbodyInputs, GameComponent&>},
      {CLASS_NAME_ID(SphereCollider), dr_make_unique<SphereColliderInputs, GameComponent&>},
      {CLASS_NAME_ID(ScriptComponent), dr_make_unique<ScriptInputs, GameComponent&>},
      {CLASS_NAME_ID(SoundComponent), dr_make_unique<SoundInputs, GameComponent&>}
    };

    return m_factories[_component.getClassID()](_component);
  }

}