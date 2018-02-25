#include "dr_quad.h"
#include <fstream>
#include <dr_d3d_shader_bytecode.h>
#include <dr_graphics_defines.h>
#include <dr_graphics_driver.h>
using namespace driderSDK;


//TODO: Remove
std::string file2string(const char *path) {
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>()));
  return content;
}

void Quad::init()
{
  Device& device = *GraphicsDriver::getApiReference().device;
  std::string vsSource = file2string("vs.hlsl");
  std::string fsSource = file2string("fs.hlsl");
  vs = reinterpret_cast<Shader*>(device.createShaderFromMemory(vsSource.c_str(), vsSource.size(),DR_SHADER_TYPE_FLAG::kVertex));
  fs = reinterpret_cast<Shader*>(device.createShaderFromMemory(fsSource.c_str(), fsSource.size(), DR_SHADER_TYPE_FLAG::kFragment));

  m_vertex[0] = { -1.f,  1.f, 0.9f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f  ,0.0,0.0 };
  m_vertex[1] = { -1.f, -1.f, 0.9f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f  ,0.0,1.0 };
  m_vertex[2] = { 1.f, -1.f, 0.9f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f  ,1.0,1.0 };
  m_vertex[3] = { 1.f,  1.f, 0.9f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f  ,1.0,0.0 };

  m_index[0] = 2;
  m_index[1] = 1;
  m_index[2] = 0;
  m_index[3] = 0;
  m_index[4] = 3;
  m_index[5] = 2;

  std::vector<DrInputElementDesc> idesc = vs->reflect();
  IL = device.createInputLayout(idesc, *vs->m_shaderBytecode);


  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);

  bdesc.type = DR_BUFFER_TYPE::kVERTEX;
  bdesc.sizeInBytes = sizeof(vertex) * 4;
  bdesc.stride = sizeof(vertex);
  VB = reinterpret_cast<VertexBuffer*>(device.createBuffer( bdesc, reinterpret_cast<byte*>(&m_vertex[0])));

  bdesc.type = DR_BUFFER_TYPE::kINDEX;
  bdesc.sizeInBytes = 6 * sizeof(UInt32);
  bdesc.stride = 0;
  IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<byte*>(&m_index[0])));

  //constBuff.WVP = Matrix4x4::identityMat4x4;

  driderSDK::DrTextureDesc tDesc;
  tDesc.width = width;
  tDesc.height = height;
  tDesc.pitch = width * 4;
  tDesc.Format = DR_FORMAT::kDrFormat_B8G8R8A8_UNORM;
  tDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | DR_BIND_FLAGS::RENDER_TARGET;
  tDesc.mipLevels = 0;
  tDesc.dimension = DR_DIMENSION::k2D;
  tDesc.genMipMaps = true;
  texture = device.createEmptyTexture(tDesc);

  driderSDK::DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kANISOTROPIC;
  SSdesc.maxAnisotropy = 16;
  SS = device.createSamplerState(SSdesc);
}

void Quad::destroy()
{
  CB->release();
  IB->release();
  VB->release();
}

void Quad::draw()
{ 
  //constBuff.WVP = wvp;
  DeviceContext& deviceContext = *GraphicsDriver::getApiReference().deviceContext;
  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);
  VB->set(deviceContext);
  IB->set(deviceContext); 
  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);
  texture->set(deviceContext, 0);
  SS->set(deviceContext,DR_SHADER_TYPE_FLAG::kFragment);
  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  deviceContext.draw(6, 0, 0);
}
