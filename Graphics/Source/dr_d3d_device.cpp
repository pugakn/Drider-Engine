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
namespace driderSDK {
  DR_GRAPHICS_ERROR::E D3DDevice::createDeviceAndDeviceContext(DeviceContext& deviceContext)
  {
    if (D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, D3D11Device.GetAddressOf(), 0, static_cast<D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext.GetAddressOf()) != S_OK)
      return DR_GRAPHICS_ERROR::CREATE_DEVICE_ERROR;
    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  void D3DDevice::release()
  {
    D3D11Device.Reset();
  }
  DR_GRAPHICS_ERROR::E D3DDevice::createVertexBuffer(const DrBufferDesc& desc,
                                    char * initialData, 
                                    VertexBuffer& vertexBuffer) 
{
  return vertexBuffer.create(*this,desc,initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createIndexBuffer(const DrBufferDesc & desc, char * initialData, IndexBuffer & indexBuffer)
{
  return indexBuffer.create(*this, desc, initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createConstantBuffer(const DrBufferDesc & desc, char * initialData, ConstantBuffer & constantBuffer)
{
  return constantBuffer.create(*this, desc, initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createShaderFromMemory(const char * shaderBuffer,size_t bufferSize, Shader & shader)
{
  return shader.createFromMemory(*this,shaderBuffer,bufferSize);
}

DR_GRAPHICS_ERROR::E D3DDevice::createTextureFromMemory(const char * buffer, const DrTextureDesc & desc, Texture & texture)
{
  return texture.createFromMemory(*this, desc, buffer);
}

DR_GRAPHICS_ERROR::E D3DDevice::createEmptyTexture(const DrTextureDesc & desc, Texture & texture)
{
  return texture.createEmpty(*this, desc);
}

DR_GRAPHICS_ERROR::E D3DDevice::createRenderTarget(const DrTextureDesc & desc, RenderTarget & renderTarget, UInt32 _numColorTextures)
{
  return renderTarget.create(*this,desc,_numColorTextures);
}

DR_GRAPHICS_ERROR::E D3DDevice::createDepthStencil(const DrTextureDesc & desc, DepthStencil & depthStencil)
{
  return depthStencil.create(*this,desc);
}

DR_GRAPHICS_ERROR::E D3DDevice::createSamplerState(const DrSampleDesc & desc, SamplerState & state)
{
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E D3DDevice::createRasteizerState(const DrRasterizerDesc & desc, RasterizerState & state)
{
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E D3DDevice::createDepthStencilState(const DrDepthStencilDesc & desc, DepthStencilState & state)
{
  return state.create(*this, desc);
}

DR_GRAPHICS_ERROR::E D3DDevice::createInputLayout(const DrInputElementDesc * inputDescArray, UInt32 arraySize, InputLayout & layout)
{
  return layout.create(*this, inputDescArray, arraySize);
}

DR_GRAPHICS_ERROR::E D3DDevice::createSwapChain(const DrSwapChainDesc & desc, SwapChain & swapChain)
{
  return swapChain.create(*this, desc);
}

}



