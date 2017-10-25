#pragma once
#include <dr_prerequisites.h>
#include <memory>
#include "dr_texture.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT RenderTarget {
public:
  virtual ~RenderTarget() {};
  virtual DR_GRAPHICS_ERROR::E create(const Device& device, const DrTextureDesc& colorDesc, const DrTextureDesc& depthDesc) = 0;
  virtual void set(const DeviceContext& deviceContext) const = 0;
  virtual void release() = 0;
  std::unique_ptr<Texture> colorTexture;
  std::unique_ptr<Texture> depthTexture;
};
}
