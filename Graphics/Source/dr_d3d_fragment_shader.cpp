#include "dr_d3d_fragment_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"

namespace driderSDK {

void*
D3DFragmentShader::getAPIObject() {
  return APIShader;
}

void**
D3DFragmentShader::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APIShader);
}

void
D3DFragmentShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      PSSetShader(APIShader, 0, 0);
}

void
D3DFragmentShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete m_shaderBytecode;
  delete this;
}

void
D3DFragmentShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  HRESULT hr = reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreatePixelShader(apiShaderBytcode->GetBufferPointer(),
                      apiShaderBytcode->GetBufferSize(),
                      0,
                      &APIShader);
}


void
D3DFragmentShader::compile(const Device&,
                           const char* buffer,
                           size_t bufferSize) {
  m_shaderBytecode = new D3DShaderBytecode();
  HRESULT hr = D3DCompile(buffer,
                          bufferSize,
                          0,
                          0,
                          0,
                          "FS",
                          "ps_5_0",
                          0,
                          0,
                          &reinterpret_cast<D3DShaderBytecode*>
                            (m_shaderBytecode)->shader_blob,
                          0);
}

}