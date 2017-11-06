#pragma once
#include "dr_vertex_shader.h"

struct ID3D11VertexShader;

namespace driderSDK {

/**
* Class for vertex shader
*
* Sample usage:
*	VertexShader* = new D3DVertexShader;
*/
class DR_API_EXPORT D3DVertexShader : public VertexShader
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
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11VertexShader* APIShader;
};

}