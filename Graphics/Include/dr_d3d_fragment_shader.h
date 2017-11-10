#pragma once
#include "dr_fragment_shader.h"
#include <d3d11.h>

namespace driderSDK {

/**
* Class for pixel shader
*
* Sample usage:
*	FragmentShader* = new D3DFragmentShader;
*/
class DR_API_EXPORT D3DFragmentShader : public FragmentShader
{
 public:
  /**
  * Set the shader to the stage
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  */
  void
  set(const DeviceContext& deviceContext) const override;

  /**
  * Release the allocated memory
  */
  void
  release() override;

  /**
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
  DR_GRAPHICS_ERROR::E
  create(const Device& device) override;

  DR_GRAPHICS_ERROR::E
  compile(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11PixelShader* APIShader;
  ID3DBlob* shader_blob;
};

}