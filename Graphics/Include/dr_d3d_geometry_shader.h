#pragma once
#include "dr_geometry_shader.h"
#include <d3d11.h>

namespace driderSDK {

/**
* Class for geometry shader
*
* Sample usage:
*	Geometryshader* = new D3DGeometryShader;
*/
class DR_GRAPHICS_EXPORT D3DGeometryShader : public GeometryShader
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

  ID3D11GeometryShader* APIShader;
};

}