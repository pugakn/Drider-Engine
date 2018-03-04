#pragma once
#include "dr_script_prerequisites.h"

namespace driderSDK {

class DR_SCRIPT_EXPORT ObjectAS {
public:
  Int32 refCount = 0;

  ObjectAS() {
    refCount = 1;
  }

  void AddRef() {
    refCount++;
  }
  void Release() {
    if (--refCount == 0)
      delete this;
  }

  float Add(float param1) {
    return param1;
  }
};

}