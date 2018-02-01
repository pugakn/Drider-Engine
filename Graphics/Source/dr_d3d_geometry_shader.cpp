#include "dr_d3d_geometry_shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"

namespace driderSDK {

void*
D3DGeometryShader::getAPIObject() {
  return APIShader;
}

void**
D3DGeometryShader::getAPIObjectReference() {
  return reinterpret_cast<void**>(&APIShader);
}

void
D3DGeometryShader::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      GSSetShader(APIShader, 0, 0);
}

void
D3DGeometryShader::release() {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  APIShader->Release();
  apiShaderBytcode->Release();
  delete m_shaderBytecode;
  delete this;
}

void 
D3DGeometryShader::create(const Device& device) {
  ID3DBlob* apiShaderBytcode = reinterpret_cast<D3DShaderBytecode*>
                                 (m_shaderBytecode)->shader_blob;
  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
      CreateGeometryShader(apiShaderBytcode->GetBufferPointer(),
                           apiShaderBytcode->GetBufferSize(),
                           0,
                           &APIShader);
  
}

void
D3DGeometryShader::compile(const Device& device,
                           const char* buffer,
                           size_t bufferSize) {
  m_shaderBytecode = new D3DShaderBytecode();
  D3DCompile(buffer,
             bufferSize,
             0,
             0,
             0,
             "GS",
             "gs_5_0",
             0,
             0,
             &reinterpret_cast<D3DShaderBytecode*>(m_shaderBytecode)->shader_blob,
             0);
}

}
