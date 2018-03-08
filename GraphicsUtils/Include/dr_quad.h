#pragma once
#include <vector>
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_index_buffer.h>
#include <dr_d3d_constant_buffer.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_vertex_shader.h>
#include <dr_d3d_fragment_shader.h>
#include <dr_d3d_input_layout.h>

struct vertex
{
  float x, y, z, w;
  float nx, ny,nz,nw;
  float u, v;
};
struct CBuffer {
  float xd[16];
};
class Quad {
 public:
  void init();
  void destroy();
  void draw();
  vertex m_vertex[4];
  driderSDK::UInt32 m_index[6];
  CBuffer constBuff;
  driderSDK::VertexBuffer* VB;
  driderSDK::IndexBuffer* IB;
  driderSDK::ConstantBuffer* CB;

  driderSDK::Shader* vs;
  driderSDK::Shader* fs;

  driderSDK::InputLayout* IL;
  
};