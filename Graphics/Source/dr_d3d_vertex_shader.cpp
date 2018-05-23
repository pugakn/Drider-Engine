#include "dr_d3d_vertex_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
#include <iostream>
namespace driderSDK {

void*
D3DVertexShader::getAPIObject() {
  return APIShader;
}

void**
D3DVertexShader::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APIShader);
}

void
D3DVertexShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      VSSetShader(APIShader, 0, 0);
}

void
D3DVertexShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete m_shaderBytecode;
  delete this;
}

void
D3DVertexShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateVertexShader(apiShaderBytcode->GetBufferPointer(),
                       apiShaderBytcode->GetBufferSize(),
                       0,
                       &APIShader);
}


void
D3DVertexShader::compile(const Device&,
                         const char* buffer,
                         size_t bufferSize) {
  m_shaderBytecode = new D3DShaderBytecode();
  ID3DBlob* errorBlob = nullptr;
  auto pp = D3DCompile(buffer,
                       bufferSize,
                       0,
                       0,
                       0,
                       "VS",
                       "vs_5_0",
                       0,
                       0,
                       &reinterpret_cast<D3DShaderBytecode*>
                         (m_shaderBytecode)->shader_blob,
                       &errorBlob);
  if (errorBlob) {
    std::cout << (char*)errorBlob->GetBufferPointer() << std::endl;
    errorBlob->Release();
  }
}

}