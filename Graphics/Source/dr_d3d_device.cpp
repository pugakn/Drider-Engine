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

namespace driderSDK {

DR_GRAPHICS_ERROR::E D3DDevice::createDeviceAndDeviceContext(DeviceContext& deviceContext) {
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
}

DR_GRAPHICS_ERROR::E
D3DDevice::createVertexBuffer(const DrBufferDesc& desc,
                                  byte* initialData, 
                                  VertexBuffer& vertexBuffer) {
  VertexBuffer *vb = &vertexBuffer;
  vb = new D3DVertexBuffer;
  return vertexBuffer.create(*this,desc,initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createIndexBuffer(const DrBufferDesc& desc,
  byte* initialData,
                             IndexBuffer& indexBuffer) {
  IndexBuffer *ib = &indexBuffer;
  ib = new D3DIndexBuffer;
  return indexBuffer.create(*this, desc, initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createConstantBuffer(const DrBufferDesc& desc,
  byte* initialData,
                                ConstantBuffer& constantBuffer) {
  ConstantBuffer *cb = &constantBuffer;
  cb = new D3DConstantBuffer;
  return constantBuffer.create(*this, desc, initialData);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createShaderFromMemory(const char* shaderBuffer,
                                  size_t bufferSize,
                                  DR_SHADER_TYPE_FLAG::E shaderType,
                                  Shader& shader) {
  switch (shaderType)
  {
  case driderSDK::DR_SHADER_TYPE_FLAG::kVertex:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kFragment:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kHull:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kCompute:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kTexture:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kTeselation:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kDomain:
    break;
  case driderSDK::DR_SHADER_TYPE_FLAG::kGeometry:
    break;
  default:
    break;
  }
  shader.compile(*this, shaderBuffer, bufferSize);
  return shader.create(*this);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createTextureFromMemory(const char* buffer,
                                   const DrTextureDesc& desc,
                                   Texture& texture) {
  return texture.createFromMemory(*this, desc, buffer);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createEmptyTexture(const DrTextureDesc& desc, Texture& texture) {
  return texture.createEmpty(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createRenderTarget(const DrTextureDesc& desc,
                              RenderTarget& renderTarget,
                              UInt32 _numColorTextures) {
  return renderTarget.create(*this,);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createDepthStencil(const DrTextureDesc& desc,
                              DepthStencil& depthStencil) {
  return depthStencil.create(*this,);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createSamplerState(const DrSampleDesc& desc, SamplerState& state) {
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createRasteizerState(const DrRasterizerDesc& desc,
                                RasterizerState& state) {
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createDepthStencilState(const DrDepthStencilDesc& desc,
                                   DepthStencilState& state) {
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray,
                             const ShaderBytecode& shaderBytecode,
                             InputLayout& layout) {
  return layout.create(*this, inputDescArray, shaderBytecode);
}

DR_GRAPHICS_ERROR::E
D3DDevice::createSwapChain(const DrSwapChainDesc& desc, SwapChain& swapChain) {
  return swapChain.create(*this, desc);
}

}