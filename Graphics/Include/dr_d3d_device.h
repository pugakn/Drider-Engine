#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_device.h"

class ID3D11Device;

namespace driderSDK {
class ShaderBytecode;
class DR_GRAPHICS_EXPORT D3DDevice : public Device
{
 public:
  /**
  * TEST::
  *
  */
  DR_GRAPHICS_ERROR::E
  createDeviceAndDeviceContext(DeviceContext& deviceContext) override;

  /**
  * TEST::
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * TEST::createVertexBuffer
  *
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
  Buffer*
  createBuffer(const DrBufferDesc& desc,
                     byte* initialData = nullptr) override;

  /**
  * TEST::createShaderFromMemory
  *
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
  Shader*
  createShaderFromMemory(const char* shaderBuffer,
                         size_t bufferSize,
                         DR_SHADER_TYPE_FLAG::E shaderType) override;

  /**
  * TEST::createTextureFromMemory
  *
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
  Texture*
  createTextureFromMemory(const char* buffer,
                          const DrTextureDesc& desc) override;

  /**
  * TEST::createEmptyTexture
  *
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
  Texture*
  createEmptyTexture(const DrTextureDesc& desc) override;

  /**
  * TEST::createRenderTarget
  *
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
  RenderTarget*
  createRenderTarget(const Texture& texture) override;

  /**
  * TEST::createDepthStencil
  *
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
  DepthStencil*
  createDepthStencil(const Texture& texture) override;

  /**
  * TEST::createSamplerState
  *
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
  SamplerState*
  createSamplerState(const DrSampleDesc& desc) override;

  /**
  * TEST::createRasteizerState
  *
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
  RasterizerState*
  createRasteizerState(const DrRasterizerDesc& desc) override;

  /**
  * TEST::createDepthStencilState
  *
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
  DepthStencilState*
  createDepthStencilState(const DrDepthStencilDesc& desc) override;

  /**
  * TEST::createInputLayout
  *
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
  InputLayout*
  createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray,
                    const ShaderBytecode& shaderBytecode) override;

  /**
  * TEST::createSwapChain
  *
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
  SwapChain* 
  createSwapChain(const DrSwapChainDesc& desc) override;

  ID3D11Device* D3D11Device;
};

}