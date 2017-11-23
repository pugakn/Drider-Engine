#pragma once
#include "dr_texture.h"
#include <wrl.h>
#include <wrl/client.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace driderSDK {

/**
* Class for texture
*
* Sample usage:
*	texture* = new D3DTexture;
*/
class DR_GRAPHICS_EXPORT D3DTexture : public Texture
{
 public:
  /**
  * TEST::
  *
  * Create a texture with initial data
  *
  * @param device
  *   The device to create the texture
  *
  * @param desc
  *   The descriptor of the texture
  *
  * @param buffer
  *   The buffer with initial data
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device,
                   const DrTextureDesc& desc,
                   const char* buffer) override;

  /**
  * TEST::
  *
  * Create a texture
  *
  * @param device
  *   The device to create the texture
  *
  * @param desc
  *   The descriptor of the texture
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  createEmpty(const Device& device, const DrTextureDesc& desc) override;

  /**
  * TEST::
  *
  * Map the resource on a system memory buffer
  *
  * @param deviceContext
  *   The device context to get the resource
  *
  * @param buffer
  *   The buffer with initial data
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  map(const DeviceContext& deviceContext, char* buffer) override;

  /**
  * TEST::
  *
  * Set the texture to the shader
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param slot
  *   The slot where the texture will be placed
  *
  */
  void
  set(const DeviceContext& deviceContext, UInt32 slot) const override;

  /**
  * TEST::
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * TEST::
  *
  * Unmap the resource from system memory
  *
  * @param deviceContext
  *   The device context to get the resource
  */
  void
  unmap(const DeviceContext& deviceContext) override;

  /**
  * TEST::
  *
  * Update the data of the texture
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param buffer
  *   The buffer with new data
  *
  * @param buffer size
  *   The size of the buffer
  */
  void
  udpateFromMemory(const DeviceContext& deviceContext,
                   const char* buffer,
                   size_t bufferSize) override;

  /**
  * TEST::
  *
  * Generate mip maps for the current texture
  *
  * @param deviceContext
  *   The device context to set the resource
  */
  void
  generateMipMaps(const DeviceContext& deviceContext) const override;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> APITexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> APIView;
};

}