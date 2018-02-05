#include "dr_fmod_dsp.h"

namespace driderSDK {

void*
FMODDSP::getReference() {
  return dsp;
}

void**
FMODDSP::getObjectReference(){
  return reinterpret_cast<void**>(&dsp);
}

void
FMODDSP::setBypass(bool bypass) {
  result = dsp->setBypass(bypass);
}

void
FMODDSP::getBypass(bool *bypass) {
  result = dsp->getBypass(bypass);
}

}