#include "dr_d3d_sample_state.h"
#include <d3d11.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E
D3D11SamplerState::create(const Device & device, const DrSampleDesc & desc)
{
  descriptor = desc;
  D3D11_SAMPLER_DESC apiDesc;
  apiDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(desc.addressU);
  apiDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(desc.addressV);
  apiDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(desc.addressW);
  apiDesc.Filter = static_cast<D3D11_FILTER>(desc.Filter);
  apiDesc.MaxAnisotropy = desc.maxAnisotropy;
  apiDesc.BorderColor[0] = desc.borderColor[0];
  apiDesc.BorderColor[1] = desc.borderColor[1];
  apiDesc.BorderColor[2] = desc.borderColor[2];
  apiDesc.BorderColor[3] = desc.borderColor[3];
  apiDesc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(desc.comparisonFunc);
  apiDesc.MaxLOD = desc.maxLOD;
  apiDesc.MinLOD = desc.minLOD;
  apiDesc.MipLODBias = desc.mipLODBias;

  if (reinterpret_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateSamplerState(&apiDesc,&APIState) != S_OK) {
      DR_GRAPHICS_ERROR::CREATE_SAMPLER_STATE_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3D11SamplerState::set(const DeviceContext& deviceContext,
                       DR_SHADER_TYPE_FLAG::E typeFlag) const {
  const D3DDeviceContext* context = reinterpret_cast<const D3DDeviceContext*>(&deviceContext);
  if (typeFlag & DR_SHADER_TYPE_FLAG::kVertex) {
    context->D3D11DeviceContext->VSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kFragment) {
    context->D3D11DeviceContext->PSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kCompute) {
    context->D3D11DeviceContext->CSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kDomain) {
    context->D3D11DeviceContext->DSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kHull) {
    context->D3D11DeviceContext->HSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kGeometry) {
    context->D3D11DeviceContext->GSSetSamplers(0, 1, &APIState);
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTeselation) {
    //Not implemented
  }
  if (typeFlag & DR_SHADER_TYPE_FLAG::kTexture) {
    //Not implemented
  }
}

void
D3D11SamplerState::release() {
  APIState->Release();
  delete this;
}

}