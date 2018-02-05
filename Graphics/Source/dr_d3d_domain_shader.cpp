#include "dr_d3d_domain_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"

namespace driderSDK {

void*
D3DDomainShader::getAPIObject() {
  return APIShader;
}

void**
D3DDomainShader::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APIShader);
}

void
D3DDomainShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      DSSetShader(APIShader, 0, 0);
}

void
D3DDomainShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete m_shaderBytecode;
  delete this;
}

void
D3DDomainShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateDomainShader(apiShaderBytcode->GetBufferPointer(),
                       apiShaderBytcode->GetBufferSize(),
                       0,
                       &APIShader);
}

void
D3DDomainShader::compile(const Device& device,
                         const char* buffer,
                         size_t bufferSize)
{
  m_shaderBytecode = new D3DShaderBytecode();
  D3DCompile(buffer,
             bufferSize,
             0,
             0,
             0,
             "DS",
             "ds_5_0",
             0,
             0,
             &reinterpret_cast<D3DShaderBytecode*>
               (m_shaderBytecode)->shader_blob,
             0);
}

}