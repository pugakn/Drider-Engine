#pragma once
namespace driderSDK {
class SwapChain {
public:
  virtual ~SwapChain() {}
  virtual void swapBuffers() = 0;
  virtual void create() = 0;
};
}
