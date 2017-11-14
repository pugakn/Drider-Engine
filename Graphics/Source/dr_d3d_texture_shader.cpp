#include "dr_d3d_texture_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {

void
D3DTextreShader::set(const DeviceContext& deviceContext) const {
}

void
D3DTextreShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  //APIShader->Release();
  apiShaderBytcode->Release();
  delete shaderBytecode;
  delete this;
}

DR_GRAPHICS_ERROR::E
D3DTextreShader::create(const Device& device) {
  return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
}

DR_GRAPHICS_ERROR::E
D3DTextreShader::compile(const Device& device, const char* buffer, size_t bufferSize)
{
  return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
}

}