#pragma once

#include "dr_input_editor.h"

namespace driderSDK {
class PointLightInputs : public InputEditor {
 public:

  using InputEditor::InputEditor;

  void
  getInputs();

};

class DirectionalLightInputs : public InputEditor {
 public:

  using InputEditor::InputEditor;

  void
  getInputs();

};

}
