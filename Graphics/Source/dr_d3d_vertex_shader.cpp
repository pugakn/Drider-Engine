#include "dr_d3d_vertex_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

void
D3DVertexShader::set(const DeviceContext& deviceContext) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      VSSetShader(APIShader, 0, 0);
}

void
D3DVertexShader::release() {
  APIShader->Release();
}

DR_GRAPHICS_ERROR::E
D3DVertexShader::createFromMemory(const Device& device,
                                  const char* buffer,
                                  size_t bufferSize) {
  errorBlob = nullptr;
  if (D3DCompile(buffer,
                 bufferSize,
                 0,
                 0,
                 0,
                 "VS",
                 "vs_5_0",
                 0,
                 0,
                 &shader_blob,
                 &errorBlob) != S_OK) {
    if (errorBlob) {
      return DR_GRAPHICS_ERROR::COMPILE_SHADER_ERROR;
    }
  }

  if (static_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateVertexShader(shader_blob->GetBufferPointer(),
                             shader_blob->GetBufferSize(),
                             0,
                             &APIShader) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_SHADER_ERROR;
  }

  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

}