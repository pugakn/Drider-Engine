#include "dr_d3d_teselation_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {

void
D3DTeselationShader::set(const DeviceContext& deviceContext) const {
}

void
D3DTeselationShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  //APIShader->Release();
  apiShaderBytcode->Release();
  delete shaderBytecode;
  delete this;
}

DR_GRAPHICS_ERROR::E
D3DTeselationShader::create(const Device& device) {
  return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
}

DR_GRAPHICS_ERROR::E
D3DTeselationShader::compile(const Device& device, const char* buffer, size_t bufferSize)
{
  return DR_GRAPHICS_ERROR::ERROR_NOT_IMPLEMENTED;
}

}