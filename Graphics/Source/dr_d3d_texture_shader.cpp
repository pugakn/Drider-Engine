#include "dr_d3d_texture_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

void
D3DTextureShader::set(const DeviceContext& deviceContext) const {
}

void
D3DTextureShader::release() {
  //APIShader.Reset();
}

DR_GRAPHICS_ERROR::E
D3DTextureShader::createFromMemory(const Device& device,
                                  const char* buffer,
                                  size_t bufferSize) {
  return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
}

}