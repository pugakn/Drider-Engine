#pragma once

#include "dr_input_editor.h"

namespace driderSDK {
class RenderInputs : public InputEditor {
 public:
   
   using InputEditor::InputEditor;

  void
  getInputs(TString* response);
  

};

}
