#pragma once
#include <dr_prerequisites.h>
#include "dr_device.h"

class ID3D11Device;

namespace driderSDK {

class DR_API_EXPORT D3DDevice : public Device
{
public:
  DR_GRAPHICS_ERROR::E
  createDeviceAndDeviceContext(DeviceContext& deviceContext) override;

  /**
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * Create a vertex buffer
  *
  * @param DrBufferDesc
  *   Describes the buffer parameters
  *
  * @param initialData
  *   The initial data of the buffer
  *
  * @param out vertexBuffer
  *   The vertexbuffer taht will be filled
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createVertexBuffer(const DrBufferDesc& desc,
                     byte* initialData,
                     VertexBuffer& vertexBuffer) override;

  /**
  * Create a vertex buffer
  *
  * @param DrBufferDesc
  *   Describes the buffer parameters
  *
  * @param initialData
  *   The initial data of the buffer
  *
  * @param out indexBuffer
  *   The indexbuffer taht will be filled
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createIndexBuffer(const DrBufferDesc& desc,
                    byte* initialData,
                    IndexBuffer& indexBuffer) override;

  /**
  * Create a constant buffer
  *
  * @param DrBufferDesc
  *   Describes the buffer parameters
  *
  * @param initialData
  *   The initial data of the buffer
  *
  * @param out constantBuffer
  *   The constantBuffer taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createConstantBuffer(const DrBufferDesc& desc,
                       byte* initialData,
                       ConstantBuffer& constantBuffer) override;

  /**
  * Create a shader from a plain text buffer
  *
  * @param shaderBuffer
  *   The plain text shader buffer
  *
  * @param bufferSize
  *   The plain text shader buffer size
  *
  * @param out shader
  *   The shader object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createShaderFromMemory(const char* shaderBuffer,
                         size_t bufferSize,
                         DR_SHADER_TYPE_FLAG::E shaderType,
                         Shader& shader) override;

  /**
  * Create a texture from a memory buffer
  *
  * @param buffer
  *   The texture data
  *
  * @param desc
  *   Describes the texture parameters
  *
  * @param out texture
  *   The texture object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createTextureFromMemory(const char* buffer,
                          const DrTextureDesc& desc,
                          Texture& texture) override;

  /**
  * Create a texture without initial data
  *
  * @param desc
  *   Describes the texture parameters
  *
  * @param out texture
  *   The texture object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createEmptyTexture(const DrTextureDesc& desc,
                     Texture& texture) override;

  /**
  * Create a render target
  *
  * @param desc
  *   Describes the render target parameters
  *
  * @param _numColorTextures
  *   The number of textures the RR will have
  *
  * @param out renderTarget
  *   The RenderTarget object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createRenderTarget(const DrTextureDesc& desc,
                     RenderTarget& renderTarget,
                     UInt32 _numColorTextures) override;

  /**
  * Create a depth stencil
  *
  * @param desc
  *   Describes the depth stencil parameters
  *
  * @param out depthStencil
  *   The DepthStencil object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createDepthStencil(const DrTextureDesc& desc,
                     DepthStencil& depthStencil) override;

  /**
  * Create a sampler state
  *
  * @param desc
  *   Describes the sampler parameters
  *
  * @param out state
  *   The SamplerState object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createSamplerState(const DrSampleDesc& desc,
                     SamplerState& state) override;

  /**
  * Create a sampler rasterizer state
  *
  * @param desc
  *   Describes the state parameters
  *
  * @param out state
  *   The RasterizerState object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createRasteizerState(const DrRasterizerDesc& desc,
                       RasterizerState& state) override;

  /**
  * Create a deth stencil state
  *
  * @param desc
  *   Describes the state parameters
  *
  * @param out state
  *   The DepthStencilState object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createDepthStencilState(const DrDepthStencilDesc& desc,
                          DepthStencilState& state) override;

  /**
  * Create a input layout
  *
  * @param inputDescArray
  *   An array of DrInputElementDesc that describes the input layout parameters
  *
  * @param arraySize
  *   The size of the input desc array
  *
  * @param out layout
  *   The InputLayout object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray, 
                    const ShaderBytecode& shaderBytecode,
                    InputLayout& layout) override;

  /**
  * Create a swap chain
  *
  * @param desc
  *   Describes the swap chain parameters
  *
  * @param out layout
  *   The SwapChain object taht will be filled out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createSwapChain(const DrSwapChainDesc& desc,
                  SwapChain& swapChain) override;

  ID3D11Device* D3D11Device;
};

}