#pragma once

#include "dr_buffer.h"
namespace driderSDK {
class DR_API_EXPORT ConstantBuffer : public Buffer
{
 public:
  virtual ~ConstantBuffer() {}
  virtual void set(UInt32 slot, UInt32 numBuffers, ConstantBuffer* buffer) = 0;
};
}