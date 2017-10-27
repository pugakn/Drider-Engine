#pragma once
#include "dr_geometry_shader.h"
#include <wrl.h>
#include <wrl/client.h>

class ID3D11GeometryShader;

namespace driderSDK {

/**
* Class for geometry shader
*
* Sample usage:
*	Geometryshader* = new D3DGeometryShader;
*/
class DR_API_EXPORT D3DGeometryShader : public GeometryShader
{
 public:
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
  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device,
                   const char* buffer,
                   size_t bufferSize) override;

  Microsoft::WRL::ComPtr<ID3D11GeometryShader> APIShader;
};

}