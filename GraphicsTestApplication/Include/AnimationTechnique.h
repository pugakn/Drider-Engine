#pragma once

#include "Technique.h"
#include <dr_matrix4x4.h>

namespace driderSDK {

class AnimationTechnique : public Technique {
 public:

  struct CBuffer 
  {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Matrix4x4 Bones[200];
  };

  void 
  setBones(const std::vector<Matrix4x4>& bones);

  // Inherited via Technique
  virtual void compile() override;
  virtual UInt8 * getConstBufferData() override;

  CBuffer m_cBuffer;
};

}
