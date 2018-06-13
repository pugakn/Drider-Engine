#include "dr_d3d_fragment_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"

#include <dr_defines.h>

#include <iostream> //DEBUG ONLY

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
  ID3DBlob* errorBlob = nullptr;

  UInt32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if DR_DEBUG_MODE
  flags |= D3DCOMPILE_DEBUG;
#endif

  HRESULT hr = D3DCompile(buffer,
                          bufferSize,
                          0,
                          0,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE,
                          "FS",
                          "ps_5_0",
                          flags,
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