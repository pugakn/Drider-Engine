#pragma once

#include <vector>
#include <dr_util_prerequisites.h>

namespace driderSDK {

class Shader;
class InputLayout;
class ConstantBuffer;
class DeviceContext;
class Device;

class Technique 
{
 public:
  virtual void 
  compile(Device& device) = 0;
  
  bool
  prepareForDraw(DeviceContext& deviceContext);

  void 
  destroy();

 protected:
  virtual UInt8* getConstBufferData() = 0;
 protected:
  ConstantBuffer* m_constBuffer{nullptr};
  InputLayout* m_inputLayout{nullptr};
  std::vector<Shader*> m_shaders;
};

}
