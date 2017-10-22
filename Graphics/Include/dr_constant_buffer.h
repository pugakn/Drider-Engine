#pragma once

#include "dr_buffer.h"
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT ConstantBuffer : public Buffer
{
 public:
  virtual ~ConstantBuffer() {}
  virtual void set(const DeviceContext& deviceContext, UInt32 slot, UInt32 numBuffers) = 0;
};
}