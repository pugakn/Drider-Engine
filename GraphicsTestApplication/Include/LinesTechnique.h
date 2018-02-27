#pragma once

#include <dr_matrix4x4.h>
#include <dr_memory.h>
#include "Technique.h"

namespace driderSDK {

class Model;
class Camera;
class GameObject;

class LinesTechnique : public Technique
{
 public:

  using Technique::Technique;

 private:
  struct CBuffer 
  {
    Matrix4x4 WVP;
    Matrix4x4 World;
  };
  
  virtual 
  void compile() override;

  virtual UInt8* 
  getConstBufferData() override;

  CBuffer m_cBuffer;
};

}
