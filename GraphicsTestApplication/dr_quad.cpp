#include "dr_quad.h"
#include <stdio.h>
using namespace driderSDK;


//TODO: Remove
char *file2string(const char *path) {
  FILE *fd;
  long len, r;
  char *str;
  if ((fopen_s(&fd,path, "r")) != 0) {
    fprintf(stderr, "Can't open file '%s'\n", path);
    return NULL;
  }
  fseek(fd, 0, SEEK_END);
  len = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  str = (char*)malloc(1 + len * sizeof(char));
  r = fread(str, sizeof(char), len, fd);
  str[r] = '\0';
  fclose(fd);
  return str;
}

void Quad::init(Device& device)
{
  std::string vsSource = std::string(file2string("vs.hlsl"));
  std::string fsSource = std::string(file2string("fs.hlsl"));
  vs = reinterpret_cast<VertexShader*>(device.createShaderFromMemory(vsSource.c_str(), vsSource.size(),DR_SHADER_TYPE_FLAG::kVertex));
  fs = reinterpret_cast<FragmentShader*>(device.createShaderFromMemory(fsSource.c_str(), fsSource.size(), DR_SHADER_TYPE_FLAG::kFragment));

  m_vertex[0] = { -0.1f,  0.1f, 0.0f, 0.1f,  0.0f, 1.0f,0.0f, 1.0f };
  m_vertex[1] = { -0.1f, -0.1f, 0.0f, 0.1f,  0.0f, 1.0f,0.0f, 1.0f };
  m_vertex[2] = { 0.1f, -0.f, 0.0f, 0.1f,    0.0f, 1.0f,0.0f, 1.0f };
  m_vertex[3] = { 0.1f,  0.1f, 0.0f, 0.1f,   0.0f, 1.0f,0.0f, 1.0f };

  m_index[0] = 2;
  m_index[1] = 1;
  m_index[2] = 0;
  m_index[3] = 3;
  m_index[4] = 2;
  m_index[5] = 0;

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  CB = (ConstantBuffer*)device.createBuffer(bdesc);

  bdesc.type = DR_BUFFER_TYPE::kVERTEX;
  bdesc.sizeInBytes = sizeof(vertex) * 4;
  VB = reinterpret_cast<VertexBuffer*>(device.createBuffer( bdesc, reinterpret_cast<byte*>(&m_vertex[0])));

  bdesc.type = DR_BUFFER_TYPE::kINDEX;
  bdesc.sizeInBytes = 6 * sizeof(UInt32);
  IB = reinterpret_cast<IndexBuffer*>(device.createBuffer(bdesc, reinterpret_cast<byte*>(&m_index[0])));

  //constBuff.World = Matrix4x4::identityMat4x4;
}

void Quad::destroy()
{
  CB->release();
  IB->release();
  VB->release();
}

void Quad::draw(const DeviceContext& deviceContext)
{
  VB->set(deviceContext,sizeof(vertex),0); //TODO: Remove stride param, Default offset param
  IB->set(deviceContext,0); //TODO: Default offset param, add type
  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);
  fs->set(deviceContext);
  vs->set(deviceContext);
  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  deviceContext.draw(6, 0, 0);
}
