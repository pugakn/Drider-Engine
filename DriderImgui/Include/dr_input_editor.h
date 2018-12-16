#pragma once
#include <vector>

#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include "imgui.h"

namespace driderSDK {

  class GameComponent;

  class InputEditor {
  public:

    InputEditor(GameComponent& _component);

    virtual ~InputEditor();
    /**
    * Return to inputs in component
    *
    * @param response
    *
    * @return
    *   TString whit format json for de front end
    */
    virtual void
    getInputs() = 0;

    static std::unique_ptr<InputEditor>
    createInputEditor(GameComponent &_component);

  protected:
    GameComponent& m_component;
  };
}