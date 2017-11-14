#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;
class Texture;

class DR_GRAPHICS_EXPORT SwapChain
{
 public:
  /**
  * Class virutal destructor.
  */
  virtual
  ~SwapChain() {}

  /**
  * Create a swap chain
  *
  * @param device
  *   The device to create the swap chain
  *
  * @param desc
  *   Describes the swap chain params
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSwapChainDesc& desc) = 0;

  /**
  * Get the back buffer
  *
  * @param out texture
  *   The texture to fill out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  getBackBuffer(Texture& texture) = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  /**
  * Swap back-front buffer
  */
  virtual void
  swapBuffers() = 0;

  DrSwapChainDesc descriptor;
};

}