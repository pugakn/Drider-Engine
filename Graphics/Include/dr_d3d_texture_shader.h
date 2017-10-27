#pragma once
#include "dr_texture_shader.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>

namespace driderSDK {

class DR_API_EXPORT D3DTextureShader : public TextureShader
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
  createFromMemory(const Device& device,
                   const char* buffer,
                   size_t bufferSize) override;
  //Microsoft::WRL::ComPtr<ID3D11TextureShader> APIShader;
};

}