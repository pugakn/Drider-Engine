#include "dr_d3d_rasterizer_state.h"
#include <d3d11.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
namespace driderSDK {
  DR_GRAPHICS_ERROR::E D3DRasterizerState::create(const Device& device,const DrRasterizerDesc & desc)
  {
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
    HRESULT result = static_cast<const D3DDevice*>(&device)->D3D11Device->CreateRasterizerState(&rasterizerState, APIState.GetAddressOf());
    if (FAILED(result))
    {
      return DR_GRAPHICS_ERROR::CREATE_RASTERIZER_STATE_ERROR;
    }
    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  void D3DRasterizerState::set(const DeviceContext & deviceContext)
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->RSSetState(APIState.Get());
  }
  void D3DRasterizerState::release()
  {
    APIState.Get()->Release();
  }
}