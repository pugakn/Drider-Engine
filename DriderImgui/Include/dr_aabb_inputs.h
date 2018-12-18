#pragma once

#include "dr_input_editor.h"

namespace driderSDK {
class AABBInputs : public InputEditor {
public:

  using InputEditor::InputEditor;

  void
    getInputs();

};

}