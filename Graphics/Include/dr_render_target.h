#pragma once
#include "dr_graphics_prerequisites.h"
#include <memory>
#include <vector>
#include "dr_texture.h"

namespace driderSDK {

class Device;
class DeviceContext;
class DepthStencil;

/**
* Base class for render target
*
* Sample usage:
*	RenderTarget* = new D3DRenderTarget;
*/
class DR_GRAPHICS_EXPORT RenderTarget
{
 public:
   //virtual void*
   //  getAPIObject() = 0;

   //virtual void**
   //  getAPIObjectReference() = 0;

  /**
  * Class virtual destructor.
  */
  virtual
  ~RenderTarget() {};

  /**
  * Create a render target on this object
  *
  * @param device
  *   The device to create the depth stencil
  *
  * @param desc
  *   The descriptor of the depth stencil
  *
  * @param _numColorTextures
  *   The number of textures to create for the render target
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual void
  create(const Device& device, const DrTextureDesc& desc, UInt32 numRTs) = 0;

  virtual void
  create(const Device& device, const std::vector<Texture*>& textures) = 0;

  /**
  * TEST::setRTNull
  *
  * Set the render target and depth stencil
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  * @param depthStencil
  *   The depth stencil to set with the render target
  *
  */
  virtual void
  setRTNull(const DeviceContext& deviceContext) const = 0;

  /**
  * Set the render target and depth stencil
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  * @param depthStencil
  *   The depth stencil to set with the render target
  *
  */
  virtual void
  set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;


  virtual void
  clear(const DeviceContext& deviceContext, const float color[4]) = 0;


  const DrTextureDesc& getDescriptor() const { return m_descriptor; }
  Texture& getTexture(UInt32 id) const 
  { 
    DR_ASSERT(id < m_texturesVec.size());
    return *m_texturesVec[id]; 
  }
 protected:
  DrTextureDesc m_descriptor;
  std::vector<Texture*> m_texturesVec;
  bool _textureOwner = false;
};

}