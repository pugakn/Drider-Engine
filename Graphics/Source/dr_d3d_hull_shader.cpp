#include "dr_d3d_hull_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"

namespace driderSDK {

void*
D3DHullShader::getAPIObject() {
  return APIShader;
}

void**
D3DHullShader::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APIShader);
}

void
D3DHullShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      HSSetShader(APIShader, 0, 0);
}

void
D3DHullShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete m_shaderBytecode;
  delete this;
}

void
D3DHullShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateHullShader(apiShaderBytcode->GetBufferPointer(),
      apiShaderBytcode->GetBufferSize(),
      0,
      &APIShader);
}

void
D3DHullShader::compile(const Device& device,
                       const char* buffer,
                       size_t bufferSize) {
  m_shaderBytecode = new D3DShaderBytecode();
  D3DCompile(buffer,
             bufferSize,
             0,
             0,
             0,
             "HS",
             "hs_5_0",
             0,
             0,
             &reinterpret_cast<D3DShaderBytecode*>(m_shaderBytecode)->shader_blob,
             0);
}

}