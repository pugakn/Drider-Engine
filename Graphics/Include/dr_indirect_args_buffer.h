#pragma once

#include "dr_buffer.h"

namespace driderSDK {

  class DeviceContext;

  /**
  * Base class for indirect buffer shader resource
  *
  * Sample usage:
  *	IndirectArgsBuffer* = new D3DIndirectArgsBuffer;
  */
  class DR_GRAPHICS_EXPORT IndirectArgsBuffer : public Buffer
  {
  public:
    /**
    * Virtual destructor.
    */
    virtual
      ~IndirectArgsBuffer() {}

    virtual void
      set(const DeviceContext& deviceContext, Int32 typeFlag = 0, Int32 startSlot = 0) const = 0;
  };

}