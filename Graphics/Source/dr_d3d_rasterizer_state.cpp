#include "dr_d3d_rasterizer_state.h"
#include <d3d11.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {
  void * D3DRasterizerState::getAPIObject()
  {
    return APIState;
  }
  void ** D3DRasterizerState::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&APIState);
  }
  void
D3DRasterizerState::create(const Device& device,const DrRasterizerDesc & desc)
{
  m_descriptor = desc;
  D3D11_RASTERIZER_DESC rasterizerState;
  rasterizerState.FillMode = static_cast<D3D11_FILL_MODE>(desc.fillMode);
  rasterizerState.CullMode = static_cast<D3D11_CULL_MODE>(desc.cullMode);
  rasterizerState.FrontCounterClockwise = desc.frontCounterClockwise;
  rasterizerState.DepthBias = desc.depthBias;
  rasterizerState.DepthBiasClamp = desc.depthBiasClamp;
  rasterizerState.SlopeScaledDepthBias = desc.slopeScaledDepthBias;
  rasterizerState.DepthClipEnable = desc.depthClipEnable;
  rasterizerState.ScissorEnable = desc.scissorEnable;
  rasterizerState.MultisampleEnable = desc.multisampleEnable;
  rasterizerState.AntialiasedLineEnable = desc.antialiasedLineEnable;

  reinterpret_cast<const D3DDevice*>(&device)->
                     D3D11Device->
                       CreateRasterizerState(&rasterizerState, &APIState);
}

void
D3DRasterizerState::set(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      RSSetState(APIState);
}

void
D3DRasterizerState::release() {
  APIState->Release();
  delete this;
}

}