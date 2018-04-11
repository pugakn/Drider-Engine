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
  */
  virtual void
  create(const Device& device ,const DrDepthStencilDesc& desc) = 0;

  virtual void
  create(const Device& device, const Texture& texture) = 0;

  //virtual void 
  //set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;


  virtual void 
  clear(const DeviceContext & deviceContext, const float valueDepth, const float valueStencil) = 0;

  const DrDepthStencilDesc& getDescriptor() const { return m_descriptor; }
  const Texture& getTexture() const { return *m_texture; }
 protected:
  bool _textureOwner = false;
  Texture* m_texture;
  DrDepthStencilDesc m_descriptor;
};

}