#include "dr_script_object.h"

namespace driderSDK {

ScriptObject::ScriptObject() {
  refCount = 1;
}

ScriptObject::~ScriptObject() {
  
}

ScriptObject *Ref_Factory() {
  return new ScriptObject();
}

}