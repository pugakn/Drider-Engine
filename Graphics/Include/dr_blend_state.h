#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

  class DeviceContext;
  class Device;

  class DR_GRAPHICS_EXPORT BlendState
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
      ~BlendState() {}

    /**
    * Create a rasterizer state
    *
    * @param device
    *   The device to create the state
    *
    * @param desc
    *   Describes the state params
    *
    */
    virtual void
      create(const Device& device,
        const DrBlendStateDesc& desc) = 0;

    /**
    * Set the blend state
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
    const DrBlendStateDesc& getDescriptor() const { return m_descriptor; }
  protected:
    DrBlendStateDesc m_descriptor;
  };

}