#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"
#include <string>

namespace driderSDK {

class Device;
class DeviceContext;

/**
* Base class for texture
*
* Sample usage:
*	texture* = new D3DTexture;
*/
class DR_GRAPHICS_EXPORT Texture
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
  ~Texture() {}

  /**
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
  virtual void
  createFromMemory(const Device& device, 
                   const DrTextureDesc& desc, 
                   const char* buffer) = 0;

  /**
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
  virtual void
  createEmpty(const Device& device, const DrTextureDesc& desc) = 0;

  /**
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
  virtual void
  map(const DeviceContext& deviceContext,char* buffer) = 0;

  /**
  * Set the texture to the shader
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param slot
  *   The slot where the texture will be placed
  *
  */
  virtual void
  set(const DeviceContext& deviceContext, UInt32 slot) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  /**
  * Unmap the resource from system memory
  *
  * @param deviceContext
  *   The device context to get the resource
  */
  virtual void
  unmap(const DeviceContext& deviceContext) = 0;

  /**
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
  virtual void
  udpateFromMemory(const DeviceContext& deviceContext, 
                   const char* buffer, 
                   size_t bufferSize) = 0;

  /**
  * Generate mip maps for the current texture
  *
  * @param deviceContext
  *   The device context to set the resource
  */
  virtual void
  generateMipMaps(const DeviceContext& deviceContext) const = 0;

  virtual void 
  modifyTextureParams(const Device& device,
                      const DrTextureDesc& desc) = 0;

  const DrTextureDesc& getDescriptor() const { return m_descriptor; }
  void setDescriptor(const DrTextureDesc&desc) { m_descriptor = desc; };
 protected:
   DrTextureDesc m_descriptor;
};

}