#include "dr_d3d_depth_stencil_state.h"
#include <d3d11.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E
D3DDepthStencilState::create(const Device& device,
                             const DrDepthStencilDesc& desc) {
  descriptor = desc;
  D3D11_DEPTH_STENCIL_DESC dsDesc;
  dsDesc.DepthEnable = desc.depthEnable;
  dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(desc.depthFunc);

  dsDesc.StencilEnable = desc.stencilEnable;
  dsDesc.StencilReadMask = desc.stencilReadMask;
  dsDesc.StencilWriteMask = desc.stencilWriteMask;

  /*Hardcoded*/
  dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  /*Hardcoded*/
  dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  if (reinterpret_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateDepthStencilState(&dsDesc, &APIState) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_DEPTH_STATE_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DDepthStencilState::set(const DeviceContext& deviceContext,
                          UInt32 refValue) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      OMSetDepthStencilState(APIState, refValue);
}

void
D3DDepthStencilState::release() {
  APIState->Release();
  delete this;
}

}