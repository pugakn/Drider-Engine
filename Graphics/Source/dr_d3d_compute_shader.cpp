#include "dr_d3d_compute_shader.h"
#include <D3Dcompiler.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

void
D3DComputeShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      CSSetShader(APIShader, 0, 0);
}

void
D3DComputeShader::release() {
  APIShader->Release();
}

DR_GRAPHICS_ERROR::E
D3DComputeShader::create(const Device& device) {
  if (reinterpret_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateComputeShader(shader_blob->GetBufferPointer(),
                              shader_blob->GetBufferSize(),
                              0,
                              &APIShader) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_SHADER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

DR_GRAPHICS_ERROR::E
D3DComputeShader::compile(const Device& device, const char* buffer, size_t bufferSize) 
{
  ID3DBlob* errorBlob = nullptr;
  if (D3DCompile(buffer,
    bufferSize,
    0,
    0,
    0,
    "CS",
    "cs_5_0",
    0,
    0,
    &shader_blob,
    &errorBlob) != S_OK) {
    if (errorBlob) {
      return DR_GRAPHICS_ERROR::COMPILE_SHADER_ERROR;
    }
  }
  errorBlob->Release();
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

}