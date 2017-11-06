#pragma once
#include "dr_device_context.h"

class ID3D11DeviceContext;

namespace driderSDK {

class DR_API_EXPORT D3DDeviceContext : public DeviceContext {
 public:
   /**
   * Release the allocated memory
   */
  void
  release() override;

  /**
  * Generate mip maps of the texture
  *
  * @param texture
  *   The texture to generate mipmaps
  *
  */
  void
  generateMipMaps(const Texture& texture) const override;

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
  void
  updateTextureFromMemory(Texture& texture,
                          const char* databuffer,
                          size_t bufferSize) const override;

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
  void
  updateBufferFromMemory(Buffer& buffer,
                         const char* dataBuffer,
                         size_t bufferSize) const override;

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
  void
  setRenderTarget(const RenderTarget& renderTarget,
                  const DepthStencil& depthStencil) const override;

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
  void
  setTexture(const Texture& texture, UInt32 slot) const override;

  /**
  * Set the shader to the current context
  *
  * @param shader
  *   The shader to set
  *
  */
  void
  setShader(const Shader& shader) const override;

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
  void
  setIndexBuffer(const IndexBuffer& indexBuffer,
                 UInt32 offset) const override;

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
  void
  setVertexBuffer(const VertexBuffer& buffer,
                  UInt32 stride,
                  UInt32 offset) const override;

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
  void
  setConstantBuffer(const ConstantBuffer& buffer,
                    DR_SHADER_TYPE_FLAG::E typeFlag) const override;

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
  void
  clearDepthStencilView(DepthStencil& depthstencil,
                        DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,
                        float depthValue,
                        UInt32 stencilValue) const override;

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
  void
  clearRenderTargetView(RenderTarget& renderTarget,
                        const float colorRGBA[4]) const override;

  /**
  * Set the tipe of topology to use on the draw
  *
  * @param topology
  *   The topology to use
  *
  */
  void
  setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) const override;

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
  void
  draw(UInt32 indexCount,
       UInt32 startIndexLocation,
       UInt32 startVertexLocation) const override;

  ID3D11DeviceContext* D3D11DeviceContext;
};

}