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
  LinesTechnique(Camera* _camera, GameObject* _gameObject);
 
 private:
  struct CBuffer 
  {
    Matrix4x4 WVP;
    Matrix4x4 World;
  };
  
  virtual 
  void compile(Device& device) override;

  virtual UInt8* 
  getConstBufferData() override;

  CBuffer m_cBuffer;
  Camera* m_camera;
  GameObject* m_gameObject;
};

}
