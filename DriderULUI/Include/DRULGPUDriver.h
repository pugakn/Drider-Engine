#pragma once
#include <dr_types.h>
#include <dr_device_context.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <Ultralight/platform/GPUDriver.h>
#include <map>
#include <vector>
#include <memory>

using Microsoft::WRL::ComPtr;

namespace driderSDK {

/**
 * GPUDriver implementation for Direct3D 11.
 */
class DRULGPUDriver : public ultralight::GPUDriver {
 public:

  DRULGPUDriver();

  virtual ~DRULGPUDriver();

  virtual void BeginSynchronize() override;

  virtual void EndSynchronize() override;

  virtual UInt32 NextTextureId() override;

  virtual void
  CreateTexture(UInt32 texture_id,
                ultralight::Ref<ultralight::Bitmap> bitmap) override;

  virtual void
  UpdateTexture(UInt32 texture_id,
                ultralight::Ref<ultralight::Bitmap> bitmap) override;

  virtual void
  BindTexture(UInt8 texture_unit,
              UInt32 texture_id) override;

  virtual void
  DestroyTexture(UInt32 texture_id) override;

  virtual UInt32
  NextRenderBufferId() override;

  virtual void
  CreateRenderBuffer(UInt32 render_buffer_id,
                     const ultralight::RenderBuffer& buffer) override;

  virtual void
  BindRenderBuffer(UInt32 render_buffer_id) override;

  virtual void
  SetRenderBufferViewport(UInt32 render_buffer_id,
                          UInt32 width,
                          UInt32 height) override;

  virtual void
  ClearRenderBuffer(UInt32 render_buffer_id) override;

  virtual void
  DestroyRenderBuffer(UInt32 render_buffer_id) override;

  virtual UInt32
  NextGeometryId() override;

  virtual void
  CreateGeometry(UInt32 geometry_id,
                 const ultralight::VertexBuffer& vertices,
                 const ultralight::IndexBuffer& indices);

  virtual void
  UpdateGeometry(UInt32 geometry_id,
                 const ultralight::VertexBuffer& vertices,
                 const ultralight::IndexBuffer& indices);

  virtual void
  DrawGeometry(UInt32 geometry_id,
               UInt32 indices_count,
               UInt32 indices_offset,
               const ultralight::GPUState& state);

  virtual void
  DestroyGeometry(UInt32 geometry_id) override;

  virtual void
  UpdateCommandList(const ultralight::CommandList& list) override;

  virtual bool
  HasCommandsPending() override {
    return !command_list_.empty();
  }

  virtual void
  DrawCommandList() override;

  // Public Methods

  Int32
  batch_count() const {
    return batch_count_;
  }

 protected:
  ComPtr<ID3D11PixelShader> GetShader(UInt8 shader);
  ComPtr<ID3D11InputLayout> GetVertexLayout();
  ComPtr<ID3D11SamplerState> GetSamplerState();
  ComPtr<ID3D11Buffer> GetConstantBuffer();
  void UpdateConstantBuffer(const ultralight::GPUState& state);
  
  ComPtr<ID3D11VertexShader> vertex_shader_;
  ComPtr<ID3D11InputLayout> vertex_layout_;
  ComPtr<ID3D11SamplerState> sampler_state_;
  ComPtr<ID3D11Buffer> constant_buffer_;

  UInt32 next_texture_id_ = 1;
  UInt32 next_render_buffer_id_ = 1; //Render buffer id 0 is reserved for default render target view.
  UInt32 next_geometry_id_ = 1;

  UInt32 render_buffer_width_ = 0;
  UInt32 render_buffer_height_ = 0;

  typedef std::pair<ComPtr<ID3D11Buffer>, ComPtr<ID3D11Buffer>> GeometryEntry;
  typedef std::map<UInt32, GeometryEntry> GeometryMap;
  GeometryMap geometry_;

  typedef std::pair<ComPtr<ID3D11Texture2D>, ComPtr<ID3D11ShaderResourceView>> TextureEntry;
  typedef std::map<UInt32, TextureEntry> TextureMap;
  TextureMap textures_;

  struct RenderTarget { ComPtr<ID3D11RenderTargetView> rt_view; UInt32 width; UInt32 height; };
  typedef std::map<UInt32, RenderTarget> RenderTargetMap;
  RenderTargetMap render_targets_;

  typedef std::map<ultralight::ShaderType, ComPtr<ID3D11PixelShader>> ShaderMap;
  ShaderMap shaders_;

  std::vector<ultralight::Command> command_list_;
  Int32 batch_count_;
};

}
