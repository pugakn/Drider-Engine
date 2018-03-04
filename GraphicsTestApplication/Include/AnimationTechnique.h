#pragma once

#include "Technique.h"

namespace driderSDK {

class AnimationTechnique : public Technique {
public:
  // Inherited via Technique
  virtual void compile() override;
  virtual UInt8 * getConstBufferData() override;
};

}
