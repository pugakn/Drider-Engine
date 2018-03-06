#include "dr_d3d_render_target.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_depth_stencil.h"

namespace driderSDK {

//void * D3DRenderTarget::getAPIObject()
//{
//  return RTV;
//}
//void ** D3DRenderTarget::getAPIObjectReference()
//{
//  return reinterpret_cast<void**>(&RTV);
//}

void
D3DRenderTarget::create(const Device& device,
                        const DrTextureDesc& desc, 
                        UInt32 numRTs) {
  m_descriptor = desc;
  _textureOwner = true;
  for (size_t i = 0; i < numRTs; i++)
  {
    Texture* texture;
    texture = device.createEmptyTexture(desc);
    m_texturesVec.push_back(texture);
  }
  for (auto &it : m_texturesVec) {
    ID3D11RenderTargetView* view;
    reinterpret_cast<const D3DDevice*>(&device)->
      D3D11Device->
      CreateRenderTargetView(reinterpret_cast<const D3DTexture*>(it)->
        APITexture,
        NULL,
        &view);
    RTVs.push_back(view);
  }
}

void 
D3DRenderTarget::create(const Device & device, 
                        const std::vector<Texture*>& textures)
{
  _textureOwner = false;
  m_descriptor = textures.back()->getDescriptor();
  m_texturesVec = textures;
  for (auto &it : m_texturesVec) {
    ID3D11RenderTargetView* view;
    reinterpret_cast<const D3DDevice*>(&device)->
      D3D11Device->
      CreateRenderTargetView(reinterpret_cast<const D3DTexture*>(it)->
        APITexture,
        NULL,
        &view);
    RTVs.push_back(view);
  }
}

void
D3DRenderTarget::set(const DeviceContext& deviceContext,
                     const DepthStencil& depthStencil) const {


  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      OMSetRenderTargets(RTVs.size(),
                         &RTVs[0],
                         reinterpret_cast<const D3DDepthStencil*>(&depthStencil)->
                           APIDepthView);

  D3D11_VIEWPORT viewport_RT;
  viewport_RT.TopLeftX = 0;
  viewport_RT.TopLeftY = 0;
  viewport_RT.Width = static_cast<float>(m_descriptor.width);
  viewport_RT.Height = static_cast<float>(m_descriptor.height);
  viewport_RT.MinDepth = 0;
  viewport_RT.MaxDepth = 1;

  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->RSSetViewports(1, &viewport_RT);
}

void D3DRenderTarget::clear(const DeviceContext & deviceContext, const float color[4])
{
  float rgba[4]; // Cambiar después xD
  rgba[0] = 0.0f;
  rgba[1] = 0.0f;
  rgba[2] = 1.0f;
  rgba[3] = 1.0f;

  for (int i = 0; i < RTVs.size(); i++) {
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->ClearRenderTargetView(RTVs[i], rgba);
  }

  //reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
  //  D3D11DeviceContext->ClearDepthStencilView(reinterpret_cast<const D3DDepthStencil*>(&depthStencil)->
  //    APIDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void
D3DRenderTarget::release() {
  for (auto &it : RTVs) {
    it->Release();
  }
  if (_textureOwner) {
    for (auto &it : m_texturesVec) {
      it->release();
    }
  }
  delete this;
}

}