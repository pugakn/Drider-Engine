#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;

class DR_GRAPHICS_EXPORT DepthStencilState
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;
  /**
  * Create a detph stencil state that describes the behavior
  * of the deth stencil
  *
  * @param device
  *   The device to create the depth stencil state
  *
  * @param desc
  *   The descriptor of the depth stencil state
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual void
  create(const Device& device, const DrDepthStencilStateDesc& desc) = 0;

  /**
  * Set the depth stencil state
  *
  * @param deviceContext
  *   The device context set the state
  *
  * @param refValue
  *   Ther reference value to perform against when doing a 
  *   depth-stencil test
  */
  virtual void
  set(const DeviceContext& deviceContext, UInt32 refValue) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  const DrDepthStencilStateDesc& getDescriptor() const { return m_descriptor; }
protected:
  DrDepthStencilStateDesc m_descriptor;
};

}