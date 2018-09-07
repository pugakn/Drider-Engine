#include "dr_quad.h"
#include <dr_file.h>
#include <dr_string_utils.h>
#include <dr_d3d_shader_bytecode.h>
#include <dr_graphics_defines.h>
#include <dr_graphics_driver.h>

using namespace driderSDK;

void
Quad::init() {
  Device& device = GraphicsAPI::getDevice();

  File vsFile;
  vsFile.Open("Resources\\Shaders\\vs.hlsl");
  std::string vsSource = StringUtils::toString(vsFile.GetAsString(vsFile.Size()));
  vsFile.Close();

  File fsFile;
  fsFile.Open("Resources\\Shaders\\fs.hlsl");
  std::string fsSource = StringUtils::toString(fsFile.GetAsString(fsFile.Size()));
  fsFile.Close();

  vs = reinterpret_cast<Shader*>(device.createShaderFromMemory(vsSource.c_str(),
                                                               vsSource.size(),
                                                               DR_SHADER_TYPE_FLAG::kVertex));
  fs = reinterpret_cast<Shader*>(device.createShaderFromMemory(fsSource.c_str(),
                                                               fsSource.size(),
                                                               DR_SHADER_TYPE_FLAG::kFragment));

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
}

void
Quad::destroy() {
  CB->release();
  IB->release();
  VB->release();
}

void
Quad::draw() { 
  //constBuff.WVP = wvp;
  DeviceContext& deviceContext = GraphicsAPI::getDeviceContext();
  fs->set(deviceContext);
  vs->set(deviceContext);
  IL->set(deviceContext);
  VB->set(deviceContext);

  IB->set(deviceContext); 
  CB->updateFromBuffer(deviceContext,reinterpret_cast<byte*>(&constBuff));
  CB->set(deviceContext);
  //texture->set(deviceContext, 0);
  //SS->set(deviceContext,DR_SHADER_TYPE_FLAG::kFragment);
  deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  deviceContext.draw(6, 0, 0);
}
