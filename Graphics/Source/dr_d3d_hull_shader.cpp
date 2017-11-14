#include "dr_d3d_hull_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {

void
D3DHullShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      HSSetShader(APIShader, 0, 0);
}

void
D3DHullShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete shaderBytecode;
  delete this;
}

DR_GRAPHICS_ERROR::E
D3DHullShader::create(const Device& device) {
  if (reinterpret_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateHullShader(shader_blob->GetBufferPointer(),
                           shader_blob->GetBufferSize(),
                           0,
                           &APIShader) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_SHADER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

DR_GRAPHICS_ERROR::E
D3DHullShader::compile(const Device& device, const char* buffer, size_t bufferSize)
{
  ID3DBlob* errorBlob = nullptr;
  if (D3DCompile(buffer,
    bufferSize,
    0,
    0,
    0,
    "HS",
    "hs_5_0",
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