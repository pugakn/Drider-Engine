#pragma once

#include <vector>
#include <dr_util_prerequisites.h>

namespace driderSDK {

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
  
  Technique(Camera* _camera = nullptr, GameObject* _gameObject = nullptr);

  virtual ~Technique(){}

  virtual void 
  compile() = 0;
  
  bool
  prepareForDraw();

  void
  setGameObject(GameObject* gameObject) 
  {
    m_gameObject = gameObject;
  }

  void
  setCamera(Camera* camera)
  {
    m_camera = camera;
  }

  void 
  destroy();

 protected:
  virtual UInt8* getConstBufferData() = 0;
 protected:
  ConstantBuffer* m_constBuffer{nullptr};
  InputLayout* m_inputLayout{nullptr};
  std::vector<Shader*> m_shaders;
  Camera* m_camera;
  GameObject* m_gameObject;
};

}
