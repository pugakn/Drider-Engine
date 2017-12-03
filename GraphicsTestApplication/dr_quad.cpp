#include "dr_quad.h"
#include <fstream>
#include <dr_d3d_shader_bytecode.h>
#include <dr_graphics_defines.h>

#include <dr_resource_manager.h>
#include <dr_texture_resource.h>
#include <dr_texture.h>

using namespace driderSDK;


//TODO: Remove
std::string file2string(const char *path) {
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>()));
  return content;
}

void Quad::init(Device& device)
{
  std::string vsSource = file2string("vs.hlsl");
  std::string fsSource = file2string("fs.hlsl");
  vs = reinterpret_cast<VertexShader*>(device.createShaderFromMemory(vsSource.c_str(), vsSource.size(),DR_SHADER_TYPE_FLAG::kVertex));
  fs = reinterpret_cast<FragmentShader*>(device.createShaderFromMemory(fsSource.c_str(), fsSource.size(), DR_SHADER_TYPE_FLAG::kFragment));

  m_vertex[0] = { -0.5f,  0.5f, 0.9f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f,   0.0f, 0.0f };
  m_vertex[1] = { -0.5f, -0.5f, 0.9f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f,   0.0f, 1.0f };
  m_vertex[2] = { 0.5f, -0.5f, 0.9f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f,   1.0f, 1.0f };
  m_vertex[3] = { 0.5f,  0.5f, 0.9f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f,   1.0f, 0.0f };

  /*m_vertex[0] = { -0.5f,  0.5f, 0.9f, 1.0f,    0.5f, 0.5f,0.0f, 1.0f };
  m_vertex[1] = { -0.5f, -0.5f, 0.9f, 1.0f,    0.0f, 0.0f,0.0f, 1.0f };
  m_vertex[2] = { 0.5f, -0.5f, 0.9f,  1.0f,    0.0f, 0.0f,1.0f, 1.0f };
  m_vertex[3] = { 0.5f,  0.5f, 0.9f,  1.0f,    0.0f, 1.0f,1.0f, 1.0f };*/


  m_index[0] = 0;
  m_index[1] = 1;
  m_index[2] = 2;
  m_index[3] = 2;
  m_index[4] = 3;
  m_index[5] = 0;

  std::vector<DrInputElementDesc> idesc;
  DrInputElementDesc ie;
  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 0;
  ie.semanticName = "POSITION";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 16;
  ie.semanticName = "NORMAL";
  idesc.push_back(ie);

  ie.format = DR_FORMAT::kDrFormat_R32G32_FLOAT;
  ie.offset = 32;
  ie.semanticName = "TEXCOORD";
  ie.inputSlot = 0;
  ie.semanticIndex = 0;
  idesc.push_back(ie);

  IL = device.createInputLayout(idesc, *vs->m_shaderBytecode);


  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);

  bdesc.type = DR_BUFFER_TYPE::kVERTEX;
  bdesc.sizeInBytes = sizeof(vertex) * 4;
  bdesc.stride = sizeof(vertex);
  VB = reinterpret_cast<VertexBuffer*>(device.createBuffer( bdesc, 
                                       reinterpret_cast<byte*>(
                                                &m_vertex[0])));

  bdesc.type = DR_BUFFER_TYPE::kINDEX;
  bdesc.sizeInBytes = 6 * sizeof(UInt32);
  bdesc.stride = 0;
  IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc,
                                      reinterpret_cast<byte*>
                                      (&m_index[0])));

  ResourceManager resourceManager;
  resourceManager.Init();

  std::shared_ptr<TextureResource> rTexture =
    std::dynamic_pointer_cast<TextureResource>(
    resourceManager.loadResource(_T("testImage.png")));

  DrTextureDesc tDesc;
  tDesc.bindFlags = DR_BIND_FLAGS::RENDER_TARGET;
  tDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  tDesc.dimension = DR_DIMENSION::k2D;
  tDesc.Format = DR_FORMAT::kDrFormat_R32G32B32_FLOAT;
  tDesc.genMipMaps = true;
  tDesc.height = rTexture->height;
  tDesc.mipLevels = 0;
  tDesc.pitch = 1;
  tDesc.Usage = DR_BUFFER_USAGE::kImmutable;
  tDesc.width = rTexture->width;
  texture = reinterpret_cast<Texture*>(device.createTextureFromMemory(
                                       reinterpret_cast<const char*>(
                                       rTexture->data.data()),
                                       tDesc));

  //constBuff.World = Matrix4x4::identityMat4x4;
}

void Quad::destroy()
{
  //CB->release();
  IB->release();
  VB->release();
  texture->release();
}

void Quad::draw(const DeviceContext& deviceContext)
{
  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);
  VB->set(deviceContext);
  IB->set(deviceContext);
  //texture->set(deviceContext, 0);
  //CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  //CB->set(deviceContext);
  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  deviceContext.draw(6, 0, 0);
}
