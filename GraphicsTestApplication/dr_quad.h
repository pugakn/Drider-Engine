#pragma once
#include <dr_graphics_prerequisites.h>
#include <vector>
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_index_buffer.h>
#include <dr_d3d_constant_buffer.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
//#include <dr_matrix4x4.h>
#include <dr_d3d_vertex_shader.h>
#include <dr_d3d_fragment_shader.h>

struct vertex
{
  float x, y, z, w;
  float nx, ny,nz,nw;
};
struct CBuffer {
 // driderSDK::Matrix4x4 World;
};
class Quad {
public:
  void init(driderSDK::Device& device);
  void destroy();
  void draw(const driderSDK::DeviceContext& deviceContext);
  vertex m_vertex[4];
  driderSDK::UInt32 m_index[6];
  CBuffer constBuff;
  driderSDK::VertexBuffer* VB;
  driderSDK::IndexBuffer* IB;
  driderSDK::ConstantBuffer* CB;

  driderSDK::VertexShader* vs;
  driderSDK::FragmentShader* fs;

  
};