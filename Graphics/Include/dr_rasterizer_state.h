#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class DeviceContext;
class Device;

class DR_GRAPHICS_EXPORT RasterizerState
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;

  /**
  * Class virutal destructor.
  */
  virtual
  ~RasterizerState() {}

  /**
  * Create a rasterizer state
  *
  * @param device
  *   The device to create the state
  *
  * @param desc
  *   Describes the state params
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual void
  create(const Device& device,
        const DrRasterizerDesc& desc) = 0;

  /**
  * Set the irasterizer state
  *
  * @param deviceContext
  *   The device context to set the state
  *
  */
  virtual void
  set(const DeviceContext& deviceContext) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;
  const DrRasterizerDesc& getDescriptor() const { return m_descriptor; }
protected:
  DrRasterizerDesc m_descriptor;
};

}