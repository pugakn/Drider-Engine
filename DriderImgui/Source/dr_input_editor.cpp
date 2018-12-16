#include "dr_input_editor.h"

#include <unordered_map>
#include <functional>

#include "dr_render_inputs.h"

#include <dr_gameComponent.h>
#include <dr_render_component.h>
#include <dr_id_object.h>

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
      {CLASS_NAME_ID(RenderComponent), dr_make_unique<RenderInputs, GameComponent&>}
    };

    return m_factories[_component.getClassID()](_component);
  }

}