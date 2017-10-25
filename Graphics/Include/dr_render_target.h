#pragma once
#include <dr_prerequisites.h>
#include <memory>
#include "dr_texture.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DepthStencil;
class DR_API_EXPORT RenderTarget {
public:
  virtual ~RenderTarget() {};
  virtual DR_GRAPHICS_ERROR::E create(const Device& device, const DrTextureDesc& desc) = 0;
  virtual void set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;
  virtual void release() = 0;
  std::unique_ptr<Texture> colorTexture;
};
}
