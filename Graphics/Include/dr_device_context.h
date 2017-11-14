#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class Texture;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Buffer;
class RenderTarget;
class DepthStencil;
class ConstantBuffer;

class DR_GRAPHICS_EXPORT DeviceContext
{
 public:
  /**
  * Class virtual destructor.
  */
  virtual
  ~DeviceContext() {}

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  /**
  * Generate mip maps of the texture
  *
  * @param texture
  *   The texture to generate mipmaps
  *
  */
  virtual void
  generateMipMaps(const Texture& texture) const = 0;

  /**
  * Update the data of the texture
  *
  * @param texture
  *   The Texture to update
  *
  * @param databuffer
  *   The buffer with new data
  *
  * @param buffer size
  *   The size of the buffer
  *
  */
  virtual void
  updateTextureFromMemory(Texture& texture,
                          const char* databuffer,
                          size_t bufferSize) const = 0;

  /**
  * Update the data of the texture
  *
  * @param buffer
  *   The buffer to update
  *
  * @param databuffer
  *   The buffer with new data
  *
  * @param buffer size
  *   The size of the buffer
  *
  */
  virtual void
  updateBufferFromMemory(Buffer& buffer,
                         const byte* dataBuffer) const = 0;

  /**
  * Set the render target and depth stencil
  *
  * @param renderTarget
  *   The render targt to set
  *
  * @param depthStencil
  *   The depth stencil to set with the render target
  *
  */
  virtual void
  setRenderTarget(const RenderTarget& renderTarget,
                  const DepthStencil& depthStencil) const = 0;

  /**
  * Set the texture to the shader
  *
  * @param texture
  *   The texture to set
  *
  * @param slot
  *   The slot where the texture will be placed
  *
  */
  virtual void
  setTexture(const Texture& texture,
             UInt32 slot) const = 0;

  /**
  * Set the shader to the current context
  *
  * @param shader
  *   The shader to set
  *
  */
  virtual void
  setShader(const Shader& shader) const = 0;

  /**
  * Set the index buffer to the current context
  *
  * @param indexBuffer
  *   The index buffer to set
  *
  * @param offset
  *   The offset of the first element
  *
  */
  virtual void
  setIndexBuffer(const IndexBuffer& indexBuffer, 
                 UInt32 offset) const = 0;

  /**
  * Set the vertex buffer to the current context
  *
  * @param buffer
  *   The buffer to set
  *
  * @param stride
  *   The size of one vertex data in bytes
  *
  * @param offset
  *   The offset of the first element
  *
  */
  virtual void
  setVertexBuffer(const VertexBuffer& buffer,
                  UInt32 stride, UInt32 offset) const = 0;

  /**
  * Set the constant buffer to all shaders specifyed on typeFlag
  *
  * @param buffer
  *   The buffer to set
  *
  * @param typeFlag
  *   Bit flag that specifyes the shaders to set the constant buffer
  *
  */
  virtual void
  setConstantBuffer(const ConstantBuffer& buffer,
                    DR_SHADER_TYPE_FLAG::E typeFlag) const = 0;

  /**
  * Clear the depth stencil 
  *
  * @param depthstencil
  *   The depth stencil to clear
  *
  * @param clearType
  *   specifies wich buffer clear depth, stencil or both
  *
  * @param depthValue
  *   The value to set on depth
  *
  * @param stencilValue
  *   The value to set on stencil
  *
  */
  virtual void
  clearDepthStencilView(DepthStencil& depthstencil,
                        DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,
                        float depthValue,
                        UInt32 stencilValue) const = 0;

  /**
  * Clear a render target
  *
  * @param renderTarget
  *   The renderTarget to clear
  *
  * @param colorRGBA
  *   The color of the clear
  *
  */
  virtual void
  clearRenderTargetView(RenderTarget& renderTarget,
                        const float colorRGBA[4])const = 0;

  /**
  * Set the tipe of topology to use on the draw
  *
  * @param topology
  *   The topology to use
  *
  */
  virtual void
  setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) const = 0;

  /**
  * Draw the primitives
  *
  * @param indexCount
  *   The number of index to use
  *
  * @param startIndexLocation
  *   The start index location
  *
  * @param startVertexLocation
  *   The start vertex location
  *
  */
  virtual void
  draw(UInt32 indexCount,
       UInt32 startIndexLocation,
       UInt32 startVertexLocation) const = 0;

  //TODO: Add get methods
};

}