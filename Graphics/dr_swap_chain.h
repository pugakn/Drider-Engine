#pragma once

namespace driderSDK {
class SwapChain
{
 public:
  virtual void getBackBuffer() = 0;
  virtual void resizeBuffers() = 0;
  virtual void resizeTarget() = 0;
  virtual void setFullscreenState() = 0;
  virtual void getFullscreenState() = 0;

  virtual void swapBuffers() = 0;
 private:

};
}