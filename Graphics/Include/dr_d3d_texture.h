#pragma once
#include "dr_texture.h"
#include <wrl.h>
#include <wrl/client.h>
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
namespace driderSDK{
class DR_API_EXPORT D3DTexture : public Texture{
public:
  DR_GRAPHICS_ERROR::E createFromMemory(const Device& device,const DrTextureDesc& desc, const char* buffer) override;
  DR_GRAPHICS_ERROR::E createEmpty(const Device& device, const DrTextureDesc& desc) override;
  DR_GRAPHICS_ERROR::E map(const DeviceContext & deviceContext, char* buffer) override;
  void set(const DeviceContext& deviceContext, UInt32 slot) const override;
  void release() override;
  void unmap(const DeviceContext & deviceContext) override;
  void udpateFromMemory(const DeviceContext& deviceContext, const char* buffer, size_t bufferSize) override;
  void generateMipMaps(const DeviceContext & deviceContext) const override;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> APITexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> APIView;
};
}