#pragma once

#include <dr_matrix4x4.h>
#include <dr_util_prerequisites.h>
#include "Technique.h"

namespace driderSDK {

class Camera;
class GameObject;

class StaticMeshTechnique : public Technique 
{
 public:
  struct CBuffer 
  {
    Matrix4x4 WVP;
    Matrix4x4 World;
  };

  StaticMeshTechnique(Camera* camera_, GameObject* gameObject_);

  virtual void 
  compile(Device & device) override;

  virtual UInt8* 
  getConstBufferData() override;
 private:
  CBuffer m_constBufferObj;
  Camera* m_camera;
  GameObject* m_gameObject;
};

}