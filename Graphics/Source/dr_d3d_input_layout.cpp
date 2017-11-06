#include "dr_d3d_input_layout.h"
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E
D3DInputLayout::create(const Device& device,
                       const std::vector<DrInputElementDesc>& inputDescArray) {
  descriptorVec = inputDescArray;
  std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
  desc.resize(inputDescArray.size());

  for (size_t i = 0; i < inputDescArray.size(); i++) {
    desc[i].Format = (DXGI_FORMAT)inputDescArray[i].format;
    desc[i].AlignedByteOffset = inputDescArray[i].offset;
    desc[i].SemanticIndex = inputDescArray[i].semanticIndex;
    desc[i].SemanticName = inputDescArray[i].semanticName;
    desc[i].SemanticIndex = inputDescArray[i].inputSlot;
  }

  if (static_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateInputLayout(&desc[0], inputDescArray.size(), 0, 0, &APILayout) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_INPUT_LAYOUT_ERROR;
  }

  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DInputLayout::set(const DeviceContext & deviceContext) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      IASetInputLayout(APILayout);
}

void
D3DInputLayout::release()
{
  APILayout->Release();
}

}