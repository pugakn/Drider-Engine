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
  virtual 
  void compile() override;
 private:
  struct CBuffer 
  {
    Matrix4x4 WVP;
    Matrix4x4 World;
  };
  


  virtual UInt8* 
  getConstBufferData() override;

  CBuffer m_cBuffer;
};

}
