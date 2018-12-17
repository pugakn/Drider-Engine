#pragma once

#include "dr_input_editor.h"

namespace driderSDK {
class BoxColliderInputs : public InputEditor {
 public:

  using InputEditor::InputEditor;

  void
  getInputs();

};

}
