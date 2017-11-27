#include "dr_d3d_input_layout.h"
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_vertex_shader.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {
  void * D3DInputLayout::getAPIObject()
  {
    return APILayout;
  }
  void ** D3DInputLayout::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&APILayout);
  }
  void
D3DInputLayout::create(const Device& device,
                       const std::vector<DrInputElementDesc>& inputDescArray,
                       const ShaderBytecode& shaderBytecode ) {
  const D3DShaderBytecode* apiShaderBytecode = reinterpret_cast<const D3DShaderBytecode*>(&shaderBytecode);
  m_descriptorVec = inputDescArray;
  std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
  desc.resize(inputDescArray.size());

  for (size_t i = 0; i < inputDescArray.size(); i++) {
    desc[i].Format = (DXGI_FORMAT)inputDescArray[i].format;
    desc[i].AlignedByteOffset = inputDescArray[i].offset;
    desc[i].SemanticIndex = inputDescArray[i].semanticIndex;
    desc[i].SemanticName = inputDescArray[i].semanticName;
    desc[i].InputSlot = inputDescArray[i].inputSlot;
  }

  HRESULT HR = reinterpret_cast<const D3DDevice*>(&device)->D3D11Device->
    CreateInputLayout(&desc[0], inputDescArray.size(),
      apiShaderBytecode->shader_blob->GetBufferPointer(),
      apiShaderBytecode->shader_blob->GetBufferSize(), &APILayout);

  
}

void
D3DInputLayout::set(const DeviceContext & deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->IASetInputLayout(APILayout);
}

void
D3DInputLayout::release()
{
  APILayout->Release();
  delete this;
}

}