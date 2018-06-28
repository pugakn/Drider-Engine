#pragma once
#include <dr_util_prerequisites.h>
namespace driderSDK {
  class InputEditor {
  public:
    virtual void getInputs(TString* response) = 0;
  }
}