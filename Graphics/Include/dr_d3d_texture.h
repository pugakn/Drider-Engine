#pragma once
#include "dr_texture.h"


struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
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
   void*
     getAPIObject() override;

   void**
     getAPIObjectReference() override;

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
  void
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
  void
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
  void
  map(const DeviceContext& deviceContext, char** buffer) override;


  void
  getMemoryBuffer(const DeviceContext& deviceContext, std::vector<byte>& buff) override;

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
  * @param shaderType
  *   The shader where the texture will be set on
  *
  */
  void
  set(const DeviceContext& deviceContext,
      UInt32 slot,
      DR_SHADER_TYPE_FLAG::E shaderType = DR_SHADER_TYPE_FLAG::kFragment) const override;

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
                   size_t bufferSize,
                   size_t mipLevels = 1) override;

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

  void
  modifyTextureParams(const Device& device,
                       const DrTextureDesc& desc);

  ID3D11Texture2D* APITexture;
  ID3D11ShaderResourceView* APIView;
  ID3D11UnorderedAccessView* m_APIUAV;
 private:
  ID3D11Texture2D* m_stagingTexture;
  UInt32 m_arraySize;
};

}