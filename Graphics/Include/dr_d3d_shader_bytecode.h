#pragma once
#include "dr_shader_bytecode.h"
#include <d3d11.h>
namespace driderSDK {
class DR_GRAPHICS_EXPORT D3DShaderBytecode : 
  public ShaderBytecode
{
public:
  ID3DBlob* shader_blob;
};
}