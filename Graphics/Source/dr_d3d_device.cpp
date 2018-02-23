#include "dr_d3d_device.h"
#include <iostream>
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_index_buffer.h"
#include "dr_d3d_constant_buffer.h"
#include "dr_shader.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_render_target.h"
#include "dr_d3d_depth_stencil.h"
#include "dr_d3d_sample_state.h"
#include "dr_d3d_depth_stencil_state.h"
#include "dr_d3d_rasterizer_state.h"
#include "dr_d3d_input_layout.h"
#include "dr_d3d_swap_chain.h"
#include "dr_d3d_shader_bytecode.h"

#include "dr_d3d_vertex_shader.h"
#include "dr_d3d_fragment_shader.h"
#include "dr_d3d_compute_shader.h"
#include "dr_d3d_hull_shader.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_geometry_shader.h"
#include "dr_d3d_domain_shader.h"

namespace driderSDK {
  void * D3DDevice::getAPIObject()
  {
    return D3D11Device;
  }
  void ** D3DDevice::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&D3D11Device);
  }
  void
D3DDevice::createDeviceAndDeviceContext(DeviceContext& deviceContext) {
  D3D_FEATURE_LEVEL lvl = D3D_FEATURE_LEVEL_11_0;
  D3D_FEATURE_LEVEL lvlRet = D3D_FEATURE_LEVEL_11_0;
  if (D3D11CreateDevice(0,
    D3D_DRIVER_TYPE_HARDWARE,
    0,
    0,
    &lvl,
    1,
    D3D11_SDK_VERSION,
    &D3D11Device,
    &lvlRet,
    &reinterpret_cast<D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext) != S_OK) {

      throw "Error: createDeviceAndDeviceContext";
  }
  
}

void
D3DDevice::release() {
  D3D11Device->Release();
  delete this;
}

Buffer*
D3DDevice::createBuffer(const DrBufferDesc& desc,
                        byte* initialData) {
  Buffer* buffer = nullptr;
  switch (desc.type)
  {
  case DR_BUFFER_TYPE::kVERTEX:
    buffer = new D3DVertexBuffer;
    break;
  case DR_BUFFER_TYPE::kINDEX:
    buffer = new D3DIndexBuffer;
    break;
  case DR_BUFFER_TYPE::kCONSTANT:
    buffer = new D3DConstantBuffer;
    break;
  }

  buffer->create(*this,desc,initialData);
  return buffer;
}

Shader*
D3DDevice::createShaderFromMemory(const char* shaderBuffer,
                                  size_t bufferSize,
                                  DR_SHADER_TYPE_FLAG::E shaderType) {
  Shader* shader = nullptr;
  switch (shaderType)
  {
  case driderSDK::DR_SHADER_TYPE_FLAG::kVertex:
    shader = new D3DVertexShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kFragment:
    shader = new D3DFragmentShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kHull:
    shader = new D3DHullShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kCompute:
    shader = new D3DComputeShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kTexture:
    return nullptr;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kTeselation:
    return nullptr;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kDomain:
    shader = new D3DDomainShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kGeometry:
    shader = new D3DGeometryShader;
    break;
  }
  shader->compile(*this, shaderBuffer, bufferSize);
  shader->create(*this);
  return shader;
}

Texture*
D3DDevice::createTextureFromMemory(const char* buffer,
                                   const DrTextureDesc& desc) {
  Texture* texture = new D3DTexture;
  texture->createFromMemory(*this, desc, buffer);
  return texture;
}

Texture*
D3DDevice::createEmptyTexture(const DrTextureDesc& desc) {
  Texture* texture = new D3DTexture;
  texture->createEmpty(*this, desc);
  return texture;
}

RenderTarget* 
D3DDevice::createRenderTarget(const Texture& texture) {
  RenderTarget* renderTarget = new D3DRenderTarget;
  renderTarget->create(*this,texture);
  return renderTarget;
}

DepthStencil* 
D3DDevice::createDepthStencil(const Texture& texture) {
  DepthStencil* depthStencil = new D3DDepthStencil;
  depthStencil->create(*this,texture);
  return depthStencil;
}

SamplerState* 
D3DDevice::createSamplerState(const DrSampleDesc& desc) {
  SamplerState* state = new D3D11SamplerState;
  state->create(*this, desc);
  return state;
}

RasterizerState* 
D3DDevice::createRasteizerState(const DrRasterizerDesc& desc) {
  RasterizerState* state = new D3DRasterizerState;
  state->create(*this, desc);
  return state;
}

DepthStencilState* 
D3DDevice::createDepthStencilState(const DrDepthStencilDesc& desc) {
  DepthStencilState* state = new D3DDepthStencilState;
  state->create(*this, desc);
  return state;
}

InputLayout*
D3DDevice::createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray,
                             const ShaderBytecode& shaderBytecode) {
  InputLayout* layout = new D3DInputLayout;
  layout->create(*this, inputDescArray, shaderBytecode);
  return layout;
}

SwapChain*
D3DDevice::createSwapChain(const DrSwapChainDesc& desc) {
  SwapChain* swapChain = new D3DSwapChain;
  swapChain->create(*this, desc);
  return swapChain;
}

}