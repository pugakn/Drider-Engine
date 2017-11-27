#pragma once
#include "dr_graphics_prerequisites.h"
#include <memory>
#include "dr_texture.h"

namespace driderSDK {

/**
* Base class for depth stencil
*
* Sample usage:
*	DepthStencil* = new D3DdepthStencil;
*/
class DR_GRAPHICS_EXPORT DepthStencil
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
  ~DepthStencil() {};

  /**
  * Create a depth stencil on this object
  *
  * @param device
  *   The device to create the depth stencil
  *
  * @param desc
  *   The descriptor of the depth stencil
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual void
  create(const Device& device ,const Texture& texture) = 0;

  //virtual void 
  //set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;
};

}