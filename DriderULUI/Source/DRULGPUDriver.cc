#include "DRULGPUDriver.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <directxcolors.h>
#include <d3dcompiler.h>
#include <string>
#include <sstream>

#define SHADER_PATH L"Resources\\Shaders\\"

namespace {

struct Vertex_2f_4ub_2f_2f_28f {
  DirectX::XMFLOAT2 pos;
  driderSDK::UInt8 color[4];
  DirectX::XMFLOAT2 tex;
  DirectX::XMFLOAT2 obj;
  DirectX::XMFLOAT4 data_0;
  DirectX::XMFLOAT4 data_1;
  DirectX::XMFLOAT4 data_2;
  DirectX::XMFLOAT4 data_3;
  DirectX::XMFLOAT4 data_4;
  DirectX::XMFLOAT4 data_5;
  DirectX::XMFLOAT4 data_6;
};

struct Uniforms {
  DirectX::XMFLOAT4 State;
  DirectX::XMMATRIX Transform;
  DirectX::XMFLOAT4 Scalar4[2];
  DirectX::XMFLOAT4 Vector[8];
  driderSDK::UInt32 ClipSize;
  DirectX::XMMATRIX Clip[8];
};

HRESULT
CompileShaderFromFile(const driderSDK::WChar* szFileName,
                      LPCSTR szEntryPoint,
                      LPCSTR szShaderModel,
                      ID3DBlob** ppBlobOut) {
  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  //#ifdef _DEBUG
  dwShaderFlags |= D3DCOMPILE_DEBUG;
  dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
  //#endif

  ComPtr<ID3DBlob> pErrorBlob;
  std::wstring path = SHADER_PATH;
  path += szFileName;
  HRESULT hr = D3DCompileFromFile(path.c_str(),
                                  nullptr,
                                  nullptr,
                                  szEntryPoint,
                                  szShaderModel,
                                  dwShaderFlags,
                                  0,
                                  ppBlobOut,
                                  pErrorBlob.GetAddressOf());

  if (FAILED(hr) && pErrorBlob) {
    OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
  }

  return hr;
}

} //namespace (unnamed)

namespace driderSDK {

DRULGPUDriver::DRULGPUDriver() {
}

DRULGPUDriver::~DRULGPUDriver() {
}

void
DRULGPUDriver::BeginSynchronize() {
}

void
DRULGPUDriver::EndSynchronize() {
}

UInt32
DRULGPUDriver::NextTextureId() {
  return ++next_texture_id_;
}

void
DRULGPUDriver::CreateTexture(UInt32 texture_id,
                             ultralight::Ref<ultralight::Bitmap> bitmap) {
  Device& drDevice = GraphicsAPI::getDevice();
  
  auto i = textures_.find(texture_id);
  if (i != textures_.end()) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateTexture, texture id already exists.",
               L"Error",
               MB_OK);
    return;
  }

  if (bitmap->format() != ultralight::kBitmapFormat_RGBA8) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateTexture, unsupported format.",
               L"Error",
               MB_OK);
  }

  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = bitmap->width();
  desc.Height = bitmap->height();
  desc.MipLevels = desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags = 0;

  auto& texture_entry = textures_[texture_id];
  HRESULT hr;

  if (bitmap->IsEmpty()) {
    desc.BindFlags = D3D11_BIND_RENDER_TARGET |
                     D3D11_BIND_SHADER_RESOURCE;
    desc.Usage = D3D11_USAGE_DEFAULT;

    hr = context_->device()->CreateTexture2D(&desc,
                                             NULL,
                                             texture_entry.first.GetAddressOf());
  } else {
    D3D11_SUBRESOURCE_DATA tex_data;
    ZeroMemory(&tex_data, sizeof(tex_data));
    tex_data.pSysMem = bitmap->pixels();
    tex_data.SysMemPitch = bitmap->row_bytes();
    tex_data.SysMemSlicePitch = bitmap->size();

    hr = context_->device()->CreateTexture2D(&desc,
                                             &tex_data,
                                             texture_entry.first.GetAddressOf());
  }

  if (FAILED(hr)) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateTexture, unable to create texture.",
               L"Error",
               MB_OK);
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
  ZeroMemory(&srv_desc, sizeof(srv_desc));
  srv_desc.Format = desc.Format;
  srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srv_desc.Texture2D.MostDetailedMip = 0;
  srv_desc.Texture2D.MipLevels = 1;

  hr = context_->device()->CreateShaderResourceView(texture_entry.first.Get(),
                                                    &srv_desc,
                                                    texture_entry.second.GetAddressOf());
}

void
DRULGPUDriver::UpdateTexture(UInt32 texture_id,
                             ultralight::Ref<ultralight::Bitmap> bitmap) {
  auto i = textures_.find(texture_id);
  if (i == textures_.end()) {
    MessageBox(nullptr,
      L"DRULGPUDriver::UpdateTexture, texture id doesn't exist.", L"Error", MB_OK);
    return;
  }

  auto& entry = i->second;
  D3D11_MAPPED_SUBRESOURCE res;
  context_->immediate_context()->Map(entry.first.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
    &res);

  if (res.RowPitch == bitmap->row_bytes()) {
    memcpy(res.pData, bitmap->pixels(), bitmap->size());
  }
  else {
    ultralight::Ref<ultralight::Bitmap> mapped_bitmap = ultralight::Bitmap::Create(bitmap->width(),
                                                                                   bitmap->height(),
                                                                                   ultralight::kBitmapFormat_RGBA8,
                                                                                   res.RowPitch,
                                                                                   res.pData,
                                                                                   res.RowPitch* bitmap->height(),
                                                                                   false);
    ultralight::IntRect dest_rect = { 0, 0, bitmap->width(), bitmap->height() };
    mapped_bitmap->DrawBitmap(dest_rect, dest_rect, bitmap, false);
  }

  context_->immediate_context()->Unmap(entry.first.Get(), 0);
}

void
DRULGPUDriver::BindTexture(UInt8 texture_unit,
                           UInt32 texture_id) {
  auto i = textures_.find(texture_id);
  if (textures_.end() == i) {
    MessageBox(nullptr,
               L"DRULGPUDriver::BindTexture, texture id doesn't exist.",
               L"Error",
               MB_OK);
    return;
  }

  auto& entry = i->second;
  context_->immediate_context()->PSSetShaderResources(texture_unit, 1, entry.second.GetAddressOf());
}

void
DRULGPUDriver::DestroyTexture(UInt32 texture_id) {
  auto i = textures_.find(texture_id);
  if (textures_.end() != i) {
    i->second.first.Reset();
    i->second.second.Reset();

    textures_.erase(i);
  }
}

UInt32
DRULGPUDriver::NextRenderBufferId() {
  return next_render_buffer_id_++;
}

void
DRULGPUDriver::CreateRenderBuffer(UInt32 render_buffer_id,
                                  const ultralight::RenderBuffer& buffer) {
  if (render_buffer_id == 0) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateRenderBuffer, render buffer ID 0 is reserved for default render target view.",
               L"Error",
               MB_OK);
    return;
  }

  auto i = render_targets_.find(render_buffer_id);
  if (i != render_targets_.end()) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateRenderBuffer, render buffer id already exists.",
               L"Error",
               MB_OK);
    return;
  }

  auto tex_entry = textures_.find(buffer.texture_id);
  if (tex_entry == textures_.end()) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateRenderBuffer, texture id doesn't exist.",
               L"Error",
               MB_OK);
    return;
  }

  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
  ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
  renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

  ComPtr<ID3D11Texture2D> tex = tex_entry->second.first;
  auto& render_target_entry = render_targets_[render_buffer_id];
  HRESULT hr = context_->device()->CreateRenderTargetView(tex.Get(),
                                                          &renderTargetViewDesc,
                                                          render_target_entry.rt_view.GetAddressOf());
  render_target_entry.width = buffer.viewport_width;
  render_target_entry.height = buffer.viewport_height;

  if (FAILED(hr)) {
    MessageBox(nullptr,
               L"DRULGPUDriver::CreateRenderBuffer, unable to create render target.",
               L"Error",
               MB_OK);
  }
}

void
DRULGPUDriver::BindRenderBuffer(UInt32 render_buffer_id) {
  // Unbind any textures/shader resources to avoid warnings in case a render
  // buffer that we would like to bind is already bound as an input texture.
  ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
  context_->immediate_context()->PSSetShaderResources(0, 1, nullSRV);
  context_->immediate_context()->PSSetShaderResources(1, 1, nullSRV);
  context_->immediate_context()->PSSetShaderResources(2, 1, nullSRV);
  
  ID3D11RenderTargetView* target;
  if (render_buffer_id == 0) {
    target = context_->render_target_view();
    context_->screen_size(render_buffer_width_, render_buffer_height_);
  } else {
    auto i = render_targets_.find(render_buffer_id);
    if (i == render_targets_.end()) {
      MessageBox(nullptr,
                 L"DRULGPUDriver::BindRenderBuffer, render buffer id doesn't exist.",
                 L"Error",
                 MB_OK);
      return;
    }

    target = i->second.rt_view.Get();
    render_buffer_width_ = i->second.width;
    render_buffer_height_ = i->second.height;
  }

  context_->immediate_context()->OMSetRenderTargets(1, &target, nullptr);

  D3D11_VIEWPORT vp;
  ZeroMemory(&vp, sizeof(vp));
  vp.Width = (float)render_buffer_width_ * context_->scale();
  vp.Height = (float)render_buffer_height_ * context_->scale();
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  context_->immediate_context()->RSSetViewports(1, &vp);
}

void
DRULGPUDriver::SetRenderBufferViewport(UInt32 render_buffer_id,
                                       UInt32 width,
                                       UInt32 height) {
  auto& render_target_entry = render_targets_[render_buffer_id];
  render_target_entry.width = width;
  render_target_entry.height = height;
}

void
DRULGPUDriver::ClearRenderBuffer(UInt32 render_buffer_id) {
  float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  if (render_buffer_id == 0) {
    context_->immediate_context()->ClearRenderTargetView(context_->render_target_view(), color);
    return;
  }

  auto i = render_targets_.find(render_buffer_id);
  if (i == render_targets_.end()) {
    MessageBox(nullptr,
      L"DRULGPUDriver::ClearRenderBuffer, render buffer id doesn't exist.", L"Error", MB_OK);
    return;
  }

  context_->immediate_context()->ClearRenderTargetView(i->second.rt_view.Get(), color);
}

void
DRULGPUDriver::DestroyRenderBuffer(UInt32 render_buffer_id) {
  auto i = render_targets_.find(render_buffer_id);
  if (i != render_targets_.end()) {
    i->second.rt_view.Reset();
    render_targets_.erase(i);
  }
}

UInt32
DRULGPUDriver::NextGeometryId() {
  return ++next_geometry_id_;
}

void
DRULGPUDriver::CreateGeometry(UInt32 geometry_id,
                              const ultralight::VertexBuffer& vertices,
                              const ultralight::IndexBuffer& indices) {
  auto layout = GetVertexLayout();
  if (!layout)
    return;

  if (geometry_.find(geometry_id) != geometry_.end())
    return;

  GeometryEntry geometry;

  HRESULT hr;

  D3D11_BUFFER_DESC vertex_desc;
  ZeroMemory(&vertex_desc, sizeof(vertex_desc));
  vertex_desc.Usage = D3D11_USAGE_DYNAMIC;
  vertex_desc.ByteWidth = vertices.size;
  vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  D3D11_SUBRESOURCE_DATA vertex_data;
  ZeroMemory(&vertex_data, sizeof(vertex_data));
  vertex_data.pSysMem = vertices.data;

  hr = context_->device()->CreateBuffer(&vertex_desc, &vertex_data, 
    geometry.first.GetAddressOf());
  if (FAILED(hr))
    return;

  D3D11_BUFFER_DESC index_desc;
  ZeroMemory(&index_desc, sizeof(index_desc));
  index_desc.Usage = D3D11_USAGE_DYNAMIC;
  index_desc.ByteWidth = indices.size;
  index_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  index_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  D3D11_SUBRESOURCE_DATA index_data;
  ZeroMemory(&index_data, sizeof(index_data));
  index_data.pSysMem = indices.data;

  hr = context_->device()->CreateBuffer(&index_desc, &index_data, 
    geometry.second.GetAddressOf());
  if (FAILED(hr))
    return;

  geometry_.insert({ geometry_id, std::move(geometry) });
}

void
DRULGPUDriver::UpdateGeometry(UInt32 geometry_id,
                              const ultralight::VertexBuffer& vertices,
                              const ultralight::IndexBuffer& indices) {
  auto i = geometry_.find(geometry_id);
  if (i == geometry_.end()) {
    MessageBox(nullptr,
      L"DRULGPUDriver::UpdateGeometry, geometry id doesn't exist.", L"Error", MB_OK);
    return;
  }

  auto& entry = i->second;
  D3D11_MAPPED_SUBRESOURCE res;

  context_->immediate_context()->Map(entry.first.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
  memcpy(res.pData, vertices.data, vertices.size);
  context_->immediate_context()->Unmap(entry.first.Get(), 0);

  context_->immediate_context()->Map(entry.second.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
  memcpy(res.pData, indices.data, indices.size);
  context_->immediate_context()->Unmap(entry.second.Get(), 0);
}

void
DRULGPUDriver::DrawGeometry(UInt32 geometry_id,
                            UInt32 indices_count,
                            UInt32 indices_offset,
                            const ultralight::GPUState& state) {
  auto i = geometry_.find(geometry_id);
  if (i == geometry_.end())
    return;

  BindRenderBuffer(state.render_buffer_id);

  if (state.texture_1_id)
    BindTexture(0, state.texture_1_id);

  UpdateConstantBuffer(state);

  auto immediate_ctx = context_->immediate_context();

  auto& geometry = i->second;
  UINT stride = sizeof(Vertex_2f_4ub_2f_2f_28f);
  UINT offset = 0;
  immediate_ctx->IASetVertexBuffers(0, 1, geometry.first.GetAddressOf(), &stride, &offset);
  immediate_ctx->IASetIndexBuffer(geometry.second.Get(), DXGI_FORMAT_R32_UINT, 0);
  immediate_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  auto sampler_state = GetSamplerState();
  immediate_ctx->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
  immediate_ctx->VSSetShader(vertex_shader_.Get(), nullptr, 0);

  auto shader = GetShader(state.shader_type);
  immediate_ctx->PSSetShader(shader.Get(), nullptr, 0);
  immediate_ctx->VSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
  immediate_ctx->PSSetConstantBuffers(0, 1, constant_buffer_.GetAddressOf());
  immediate_ctx->DrawIndexed(indices_count, indices_offset, 0);
  batch_count_++;
}

void
DRULGPUDriver::DestroyGeometry(UInt32 geometry_id) {
  auto i = geometry_.find(geometry_id);
  if (i != geometry_.end()) {
    i->second.first.Reset();
    i->second.second.Reset();
    geometry_.erase(i);
  }
}

void
DRULGPUDriver::UpdateCommandList(const ultralight::CommandList& list) {
  if (list.size) {
    command_list_.resize(list.size);
    memcpy(&command_list_[0], list.commands, sizeof(ultralight::Command) * list.size);
  }
}

void
DRULGPUDriver::DrawCommandList() {
  if (command_list_.empty()) {
    return;
  }

  batch_count_ = 0;

  for (auto& cmd : command_list_) {
    if (cmd.command_type == ultralight::kCommandType_DrawGeometry) {
      DrawGeometry(cmd.geometry_id, cmd.indices_count, cmd.indices_offset, cmd.gpu_state);
    }
    else if (cmd.command_type == ultralight::kCommandType_ClearRenderBuffer) {
      ClearRenderBuffer(cmd.gpu_state.render_buffer_id);
    }
  }

  command_list_.clear();
}

const WChar*
GetShaderPath(ultralight::ShaderType shader) {
  switch (shader) {
  case ultralight::kShaderType_Fill: {
    return L"ps\\fill.hlsl";
  }
  default: {
    MessageBox(nullptr,
               L"GetShaderPath(): Unknown shader type.",
               L"Error",
               MB_OK);
    return L"ps\\fill.hlsl";
  }
  }
}

ComPtr<ID3D11PixelShader> DRULGPUDriver::GetShader(UInt8 shader) {
  ultralight::ShaderType shader_type = (ultralight::ShaderType)shader;
  auto i = shaders_.find(shader_type);
  if (i != shaders_.end())
    return i->second.Get();

  auto& shader_entry = shaders_[shader_type];

  HRESULT hr;

  ComPtr<ID3DBlob> ps_blob;
  hr = CompileShaderFromFile(GetShaderPath(shader_type), "PS", "ps_4_0", ps_blob.GetAddressOf());
  if (FAILED(hr)) {
    MessageBox(nullptr,
      L"DRULGPUDriver::GetShader, The HLSL file cannot be compiled. Check your working directory.", L"Error", MB_OK);
    return nullptr;
  }

  // Create the pixel shader
  hr = context_->device()->CreatePixelShader(ps_blob->GetBufferPointer(),
    ps_blob->GetBufferSize(), nullptr, shader_entry.GetAddressOf());

  if (FAILED(hr))
    return nullptr;

  return shader_entry.Get();
}

ComPtr<ID3D11InputLayout> DRULGPUDriver::GetVertexLayout() {
  if (vertex_layout_)
    return vertex_layout_;

  HRESULT hr;
  ComPtr<ID3DBlob> vs_blob;
  hr = CompileShaderFromFile(L"vs\\v2f_c4f_t2f_t2f_d28f.hlsl", "VS", "vs_4_0", vs_blob.GetAddressOf());

  // Create the vertex shader
  hr = context_->device()->CreateVertexShader(vs_blob->GetBufferPointer(),
    vs_blob->GetBufferSize(), nullptr, vertex_shader_.GetAddressOf());
  if (FAILED(hr)) {
    MessageBox(nullptr,
      L"DRULGPUDriver::GetVertexLayout, The HLSL file cannot be compiled. Check your working directory.", L"Error", MB_OK);
    return nullptr;
  }

  // Define the input layout
  const D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    5, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    6, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    7, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  // Create the input layout
  hr = context_->device()->CreateInputLayout(layout, ARRAYSIZE(layout),
    vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), vertex_layout_.GetAddressOf());

  if (FAILED(hr))
    return nullptr;

  context_->immediate_context()->IASetInputLayout(vertex_layout_.Get());

  return vertex_layout_;
}

ComPtr<ID3D11SamplerState> DRULGPUDriver::GetSamplerState() {
  if (sampler_state_)
    return sampler_state_;

  D3D11_SAMPLER_DESC sampler_desc;
  ZeroMemory(&sampler_desc, sizeof(sampler_desc));
  sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampler_desc.MinLOD = 0;
  //sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
  sampler_desc.MaxLOD = 0;
  HRESULT hr = context_->device()->CreateSamplerState(&sampler_desc, &sampler_state_);

  if (FAILED(hr)) {
    MessageBox(nullptr,
      L"DRULGPUDriver::GetSamplerState, unable to create sampler state.", L"Error", MB_OK);
  }

  return sampler_state_;
}

ComPtr<ID3D11Buffer> DRULGPUDriver::GetConstantBuffer() {
  if (constant_buffer_)
    return constant_buffer_;

  D3D11_BUFFER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.ByteWidth = sizeof(Uniforms);
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  HRESULT hr = context_->device()->CreateBuffer(&desc, nullptr, constant_buffer_.GetAddressOf());
  if (FAILED(hr)) {
    MessageBox(nullptr,
      L"DRULGPUDriver::GetConstantBuffer, unable to create constant buffer.", L"Error", MB_OK);
  }

  return constant_buffer_;
}

void DRULGPUDriver::UpdateConstantBuffer(const ultralight::GPUState& state) {
  auto buffer = GetConstantBuffer();
  Uniforms uniforms;
  uniforms.State = { 0.0, (float)render_buffer_width_, (float)render_buffer_height_, (float)context_->scale() };
  uniforms.Transform = DirectX::XMMATRIX(state.transform.data);
  uniforms.Scalar4[0] =
    { state.uniform_scalar[0], state.uniform_scalar[1], state.uniform_scalar[2], state.uniform_scalar[3] };
  uniforms.Scalar4[1] = 
    { state.uniform_scalar[4], state.uniform_scalar[5], state.uniform_scalar[6], state.uniform_scalar[7] };
  for (size_t i = 0; i < 8; ++i)
    uniforms.Vector[i] = DirectX::XMFLOAT4(state.uniform_vector[i].value);
  uniforms.ClipSize = state.clip_size;
  for (size_t i = 0; i < state.clip_size; ++i)
    uniforms.Clip[i] = DirectX::XMMATRIX(state.clip[i].data);

  D3D11_MAPPED_SUBRESOURCE res;
  context_->immediate_context()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
  memcpy(res.pData, &uniforms, sizeof(Uniforms));
  context_->immediate_context()->Unmap(buffer.Get(), 0);
}

}  // namespace ultralight
