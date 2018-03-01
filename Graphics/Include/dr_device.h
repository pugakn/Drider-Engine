#pragma once
#include "dr_graphics_prerequisites.h"
#include <vector>
#include "dr_graphics_defines.h"

namespace driderSDK {

class Buffer;
class Shader;
class Texture;
class SamplerState;
class RasterizerState;
class DepthStencilState;
class InputLayout;
class SwapChain;
class RenderTarget;
class DepthStencil;
class DeviceContext;
class ShaderBytecode;

class DR_GRAPHICS_EXPORT Device
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;

  /**
  * Class virtual destructor.
  */
  virtual
  ~Device() {}

  virtual void
  createDeviceAndDeviceContext(DeviceContext& deviceContext) = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

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
  virtual Buffer*
  createBuffer(const DrBufferDesc& desc,
               const byte* initialData = nullptr) = 0;



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
  virtual Shader*
  createShaderFromMemory(const char* shaderBuffer,
                         size_t bufferSize,
                         DR_SHADER_TYPE_FLAG::E shaderType) = 0;

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
  virtual Texture*
  createTextureFromMemory(const char* buffer,
                          const DrTextureDesc& desc) = 0;

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
  virtual Texture*
  createEmptyTexture(const DrTextureDesc& desc) = 0;

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
  virtual RenderTarget*
  createRenderTarget(const std::vector<Texture*>& textures) = 0;

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
  virtual DepthStencil*
  createDepthStencil(const Texture& texture) = 0;

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
  virtual SamplerState*
  createSamplerState(const DrSampleDesc& desc) = 0;

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
  virtual RasterizerState*
  createRasteizerState(const DrRasterizerDesc& desc) = 0;

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
  virtual DepthStencilState*
  createDepthStencilState(const DrDepthStencilDesc& desc) = 0;

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
  virtual InputLayout*
  createInputLayout(const std::vector<DrInputElementDesc>& inputDescArray, 
                    const ShaderBytecode& shaderBytecode) = 0;

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
  virtual SwapChain*
  createSwapChain(const DrSwapChainDesc& desc) = 0;
};

}