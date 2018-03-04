#include "dr_d3d_blend_state.h"
#include <dxgi.h>
#include <d3d11.h>
#include "dr_device.h"
#include "dr_device_context.h"
namespace driderSDK {
  void * D3DBlendState::getAPIObject()
  {
    return m_state;
  }
  void ** D3DBlendState::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&m_state);
  }
  void D3DBlendState::create(const Device & device, const DrBlendStateDesc & desc)
  {
    CD3D11_DEFAULT def;
    CD3D11_BLEND_DESC BlendStatedesc(def);
    BlendStatedesc.RenderTarget[0].BlendEnable = desc.blendEnable;
    BlendStatedesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)desc.blendOp;
    BlendStatedesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)desc.blendOpAlpha;
    BlendStatedesc.RenderTarget[0].DestBlend = (D3D11_BLEND)desc.destBlend;
    BlendStatedesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)desc.destBlendAlpha;
    BlendStatedesc.RenderTarget[0].SrcBlend = (D3D11_BLEND)desc.srcBlend;
    BlendStatedesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)desc.srcBlendAlpha;
    ID3D11Device* apidevice = reinterpret_cast<ID3D11Device*>(const_cast<Device*>(&device)->getAPIObject());
    apidevice->CreateBlendState(&BlendStatedesc, &m_state);
  }
  void D3DBlendState::set(const DeviceContext & deviceContext) const
  {
    ID3D11DeviceContext* apideviceContext = reinterpret_cast<ID3D11DeviceContext*>(const_cast<DeviceContext*>(&deviceContext)->getAPIObject());
    apideviceContext->OMSetBlendState(m_state, 0, 0xffffffff);
  }
  void D3DBlendState::release()
  {
    m_state->Release();
    delete this;
  }
}