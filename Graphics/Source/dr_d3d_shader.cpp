#include "dr_d3d_shader.h"
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_shader_bytecode.h"
namespace driderSDK {

std::vector<DrInputElementDesc>
D3DShader::reflect() {
  auto blob = reinterpret_cast<D3DShaderBytecode*>(m_shaderBytecode)->shader_blob;
  ID3D11ShaderReflection* reflect;
  std::vector<DrInputElementDesc> ret;
  D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflect);
  D3D11_SHADER_DESC lShaderDesc;
  reflect->GetDesc(&lShaderDesc);
  int offset = 0;
  for (unsigned i = 0; i < lShaderDesc.InputParameters; i++)
  {
    D3D11_SIGNATURE_PARAMETER_DESC desc;
    reflect->GetInputParameterDesc(i, &desc);

    DrInputElementDesc ie;
    ie.semanticName = (char*)desc.SemanticName;
    ie.semanticIndex = desc.SemanticIndex;
    ie.inputSlot = 0;
    ie.offset = offset;
    ie.inputSlot = D3D11_INPUT_PER_VERTEX_DATA;
    if (desc.Mask == 1)
    {
      if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ie.format = DR_FORMAT::kR32_UINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ie.format = DR_FORMAT::kR32_SINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ie.format = DR_FORMAT::kR32_FLOAT;
      offset += 4;
    }
    else if (desc.Mask <= 3)
    {
      if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ie.format = DR_FORMAT::kR32G32_UINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ie.format = DR_FORMAT::kR32G32_SINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ie.format = DR_FORMAT::kR32G32_FLOAT;
      offset += 8;
    }
    else if (desc.Mask <= 7)
    {
      if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ie.format = DR_FORMAT::kR32G32B32_UINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ie.format = DR_FORMAT::kR32G32B32_SINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ie.format = DR_FORMAT::kR32G32B32_FLOAT;
      offset += 12;
    }
    else if (desc.Mask <= 15)
    {
      if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ie.format = DR_FORMAT::kR32G32B32A32_UINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ie.format = DR_FORMAT::kR32G32B32A32_SINT;
      else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ie.format = DR_FORMAT::kR32G32B32A32_FLOAT;
      offset += 16;
    }

    ret.push_back(ie);
  }
  reflect->Release();
  return ret;
}

}