#pragma once
#include <dr_prerequisites.h>
namespace driderSDK {
class DR_API_EXPORT SwapChain {
public:
  virtual ~SwapChain() {}
  virtual void swapBuffers() = 0;
  virtual void create() = 0;
  virtual void release() = 0;
};
}
