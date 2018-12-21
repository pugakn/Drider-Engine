#pragma once

#include "dr_input_editor.h"
#include "dr_script_var_types.h"

namespace driderSDK {

class ScriptInputs : public InputEditor {
 public:

  using InputEditor::InputEditor;

  void
  getInputs();
  
  void 
  loadVar(void* dir, 
          Int32 type,
          const char* name);

};

}
