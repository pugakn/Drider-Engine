#pragma once
#include "dr_buffer.h"

namespace driderSDK {

class DeviceContext;

class DR_API_EXPORT CommandBuffer : public Buffer
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~CommandBuffer() {}
  //virtual void 
  //set(const DeviceContext& deviceContext, UInt32 slot, UInt32 numBuffers, DR_SHADER_TYPE_FLAG::E typeFlag) const = 0;
};
}