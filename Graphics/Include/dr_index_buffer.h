#pragma once
#include "dr_buffer.h"
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT IndexBuffer : public Buffer
{
public:
  virtual ~IndexBuffer() {}
  virtual void set(const DeviceContext& device, UInt32 offset) = 0;
};
}