#pragma once
#include <dr_prerequisites.h>
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
class DR_API_EXPORT RenderTarget {
public:
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
  virtual DR_GRAPHICS_ERROR::E 
  create(const Device& device, const DrTextureDesc& desc, UInt32 _numColorTextures) = 0;

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

  std::vector<Texture*> colorTextures;
protected:
  UInt32 numColorTextures;
};
}
