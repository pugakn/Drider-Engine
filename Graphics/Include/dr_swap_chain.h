#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;
class Texture;
class RenderTarget;

class DR_GRAPHICS_EXPORT SwapChain
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;

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
  virtual void
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
  virtual RenderTarget&
  getBackBufferRT() = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  /**
  * Resize back buffer.
  * This should be called when the application window is resized.
  */
  virtual void
  resize(const Device& device, UInt32 _w, UInt32 _h) = 0;

  /**
  * Swap back-front buffer
  */
  virtual void
  swapBuffers() = 0;

  const DrSwapChainDesc& getDescriptor() const { return m_descriptor; }
 protected:
  DrSwapChainDesc m_descriptor;
};

}