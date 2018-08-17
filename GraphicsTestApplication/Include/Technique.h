#pragma once

#include <vector>
#include <dr_util_prerequisites.h>

namespace driderSDK {

class Matrix4x4;
class Shader;
class InputLayout;
class ConstantBuffer;
class DeviceContext;
class Device;
class Camera;
class GameObject;

class Technique 
{
 public:
  
  Technique(Camera* _camera = nullptr, const Matrix4x4* _world = nullptr);

  virtual ~Technique(){}

  virtual void 
  compile() = 0;
  
  bool
  prepareForDraw();

  void
  setWorld(const Matrix4x4* matrix) 
  {
    m_world = matrix;
  }

  void
  setCamera(Camera* camera)
  {
    m_camera = camera;
  }

  void 
  release();

 protected:
  virtual UInt8* getConstBufferData() = 0;
 protected:
  ConstantBuffer* m_constBuffer{nullptr};
  InputLayout* m_inputLayout{nullptr};
  std::vector<Shader*> m_shaders;
  Camera* m_camera;
  const Matrix4x4* m_world;
};

}
