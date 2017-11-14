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
#include "dr_d3d_teselation_shader.h"
#include "dr_d3d_geometry_shader.h"
#include "dr_d3d_domain_shader.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E
D3DDevice::createDeviceAndDeviceContext(DeviceContext& deviceContext) {
  if (D3D11CreateDevice(0,
                        D3D_DRIVER_TYPE_HARDWARE,
                        0,
                        0,
                        0,
                        0,
                        D3D11_SDK_VERSION,
                        &D3D11Device,
                        0,
                        &reinterpret_cast<D3DDeviceContext*>(&deviceContext)->
                          D3D11DeviceContext) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_DEVICE_ERROR;
  }

  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DDevice::release() {
  D3D11Device->Release();
  delete this;
}

DR_GRAPHICS_ERROR::E
D3DDevice::createVertexBuffer(const DrBufferDesc& desc,
                                  byte* initialData, 
                                  VertexBuffer* vertexBuffer) {
  vertexBuffer = new D3DVertexBuffer;
  return vertexBuffer->create(*this,desc,initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createIndexBuffer(const DrBufferDesc& desc,
                             byte* initialData,
                             IndexBuffer* indexBuffer) {
  indexBuffer = new D3DIndexBuffer;
  return indexBuffer->create(*this, desc, initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createConstantBuffer(const DrBufferDesc& desc,
                                byte* initialData,
                                ConstantBuffer* constantBuffer) {
  constantBuffer = new D3DConstantBuffer;
  return constantBuffer->create(*this, desc, initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createShaderFromMemory(const char* shaderBuffer,
                                  size_t bufferSize,
                                  DR_SHADER_TYPE_FLAG::E shaderType,
                                  Shader* shader) {
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
    shader = nullptr;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kTeselation:
    shader = nullptr;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kDomain:
    shader = new D3DDomainShader;
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kGeometry:
    shader = new D3DGeometryShader;
    break;
  }
  shader->compile(*this, shaderBuffer, bufferSize);
  return shader->create(*this);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createTextureFromMemory(const char* buffer,
                                   const DrTextureDesc& desc,
                                   Texture* texture) {
  texture = new D3DTexture;
  return texture->createFromMemory(*this, desc, buffer);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createEmptyTexture(const DrTextureDesc& desc, Texture* texture) {
  texture = new D3DTexture;
  return texture->createEmpty(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createRenderTarget(const Texture& texture,
                              RenderTarget* renderTarget) {
  renderTarget = new D3DRenderTarget;
  return renderTarget->create(*this,texture);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createDepthStencil(const Texture& texture,
                              DepthStencil* depthStencil) {
  depthStencil = new D3DDepthStencil;
  return depthStencil->create(*this,texture);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createSamplerState(const DrSampleDesc& desc, SamplerState* state) {
  state = new D3D11SamplerState;
  return state->create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createRasteizerState(const DrRasterizerDesc& desc,
                                RasterizerState* state) {
  state = new D3DRasterizerState;
  return state->create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createDepthStencilState(const DrDepthStencilDesc& desc,
                                   DepthStencilState* state) {
  state = new D3DDepthStencilState;
  return state->create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray,
                             const ShaderBytecode& shaderBytecode,
                             InputLayout* layout) {
  layout = new D3DInputLayout;
  return layout->create(*this, inputDescArray, shaderBytecode);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createSwapChain(const DrSwapChainDesc& desc, SwapChain* swapChain) {
  swapChain = new D3DSwapChain;
  return swapChain->create(*this, desc);
}

}