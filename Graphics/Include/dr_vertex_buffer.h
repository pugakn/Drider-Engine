#pragma once
#include "dr_buffer.h"
namespace driderSDK {
  class DeviceContext;
  class DR_API_EXPORT VertexBuffer : public Buffer
  {
  public:
    virtual ~VertexBuffer() {}
    virtual void set(const DeviceContext& deviceContext, UInt32 stride, UInt32 offset)const = 0;
  };
}