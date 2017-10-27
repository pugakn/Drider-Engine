#include "dr_d3d_geometry_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

void
D3DGeometryShader::set(const DeviceContext& deviceContext) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      GSSetShader(APIShader.Get(), 0, 0);
}

void
D3DGeometryShader::release() {
  APIShader.Reset();
}

DR_GRAPHICS_ERROR::E 
D3DGeometryShader::createFromMemory(const Device& device,
                                    const char * buffer,
                                    size_t bufferSize) {
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
  Microsoft::WRL::ComPtr<ID3DBlob> shader_blob;
  if (D3DCompile(buffer,
                 bufferSize,
                 0,
                 0,
                 0,
                 "GS",
                 "gs_5_0",
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
          CreateGeometryShader(shader_blob->GetBufferPointer(),
                               shader_blob->GetBufferSize(),
                               0,
                               &APIShader) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_SHADER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

}