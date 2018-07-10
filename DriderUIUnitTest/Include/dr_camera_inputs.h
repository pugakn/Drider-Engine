#pragma once

#include "dr_input_editor.h"

namespace driderSDK {
class CameraInputs : public InputEditor {
 public:

  using InputEditor::InputEditor;

  void
  getInputs(TString* response);

  void
  changeValue(TString &value, TString &id);

};

}