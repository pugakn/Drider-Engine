#include "dr_d3d_fragment_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {

void
D3DFragmentShader::set(const DeviceContext & deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->PSSetShader(APIShader, 0, 0);
}

void
D3DFragmentShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete shaderBytecode;
  delete this;
}

DR_GRAPHICS_ERROR::E
D3DFragmentShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  if (reinterpret_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreatePixelShader(apiShaderBytcode->GetBufferPointer(),
                            apiShaderBytcode->GetBufferSize(),
                            0,
                            &APIShader) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_SHADER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}


DR_GRAPHICS_ERROR::E
D3DFragmentShader::compile(const Device& device, const char* buffer, size_t bufferSize)
{
  shaderBytecode = new D3DShaderBytecode();
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>(shaderBytecode)->shader_blob;
  ID3DBlob* errorBlob = nullptr;
  if (D3DCompile(buffer,
    bufferSize,
    0,
    0,
    0,
    "FS",
    "ps_5_0",
    0,
    0,
    &apiShaderBytcode,
    &errorBlob) != S_OK) {
      if (errorBlob) {
        return DR_GRAPHICS_ERROR::COMPILE_SHADER_ERROR;
      }
    }
  errorBlob->Release();
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}
}