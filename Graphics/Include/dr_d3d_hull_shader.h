#pragma once
#include <d3d11.h>
#include "dr_d3d_shader.h"

namespace driderSDK {

/**
* Class for hull shader
*
* Sample usage:
*	HullShader* = new D3DhullShader;
*/
class DR_GRAPHICS_EXPORT D3DHullShader : public D3DShader
{
 public:
   void*
     getAPIObject() override;

   void**
     getAPIObjectReference() override;

  /**
  * TEST::set
  *
  * Set the shader to the stage
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  */
  void
  set(const DeviceContext& deviceContext) const override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * TEST::createFromMemory
  *
  * Update the buffer with new data
  *
  * @param deviceContext
  *   The device context to get acces to the resource
  *
  * @param buffer
  *   The uncompiled shader buffer
  *
  * @param bufferSize
  *   The uncompiled shader buffer size
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  void
  create(const Device& device) override;

  void
  compile(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11HullShader* APIShader;
};

}