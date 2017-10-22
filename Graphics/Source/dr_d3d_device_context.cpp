#include "dr_d3d_device_context.h"
#include "dr_shader.h"
namespace driderSDK {
void D3DDeviceContext::setShader(const Shader& shader) 
{
  shader.set(*this);
}
}