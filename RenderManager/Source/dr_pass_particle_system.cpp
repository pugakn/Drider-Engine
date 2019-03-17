#include "dr_pass_particle_system.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_model.h>
#include <dr_camera_manager.h>

namespace driderSDK {

ParticleSystemPass::ParticleSystemPass() {
}

ParticleSystemPass::~ParticleSystemPass() {
}

void
ParticleSystemPass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Resources\\Shaders\\particle_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\particle_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuff);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));


  m_vertex[0] = { -1.f,  1.f, 1.f, 1.0f, };
  m_vertex[1] = { -1.f, -1.f, 1.f, 1.0f, };
  m_vertex[2] = { 1.f, -1.f,  1.f,  1.0f, };
  m_vertex[3] = { 1.f,  1.f,  1.f,  1.0f, };

  m_index[0] = 2;
  m_index[1] = 1;
  m_index[2] = 0;
  m_index[3] = 0;
  m_index[4] = 3;
  m_index[5] = 2;

  DrBufferDesc bdescInstance;
  bdescInstance.type = DR_BUFFER_TYPE::kVERTEX;
  bdescInstance.sizeInBytes = sizeof(Vector4D) * 4 ;
  bdescInstance.stride = sizeof(Vector4D);
  m_VBQUAD = dr_gfx_unique((VertexBuffer*)device.createBuffer(bdescInstance, reinterpret_cast<byte*>(&m_vertex[0])));

  bdescInstance.type = DR_BUFFER_TYPE::kINDEX;
  bdescInstance.sizeInBytes = 6 * sizeof(UInt32);
  bdescInstance.stride = 0;
  m_IBQUAD = dr_gfx_unique(reinterpret_cast<IndexBuffer*>(device.createBuffer(bdescInstance, reinterpret_cast<byte*>(&m_index[0]))));
}

void
ParticleSystemPass::draw(PassDrawData* drawData) {
  ParticleSystemDrawData* data = static_cast<ParticleSystemDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  //data->OutRt->set(dc, *data->dsOptions);
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);
  m_inputLayout->set(dc);
  //const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  //data->OutRt->clear(dc, clearColor);
  //(data->dsOptions)->clear(dc, 1, 0);

  m_cbuff.V = CameraManager::getActiveCamera()->getView();
  m_cbuff.P = CameraManager::getActiveCamera()->getProjection();
  m_cbuff.V.transpose();
  m_cbuff.P.transpose();

  m_constantBuffer->updateFromBuffer(dc,(byte*)&m_cbuff);
  m_constantBuffer->set(dc,0,0);

#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
  data->emitter->m_poolBuffer->set(GraphicsAPI::getDeviceContext(),DR_SHADER_TYPE_FLAG::kVertex, 0);
  data->emitter->m_aliveBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kVertex, 1);
#else
  data->emitter->m_renderBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kVertex,2);
#endif
  m_VBQUAD->set(dc);
  m_IBQUAD->set(dc);
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  dc.drawIndexedInstancedIndirect(*data->emitter->m_drawIndirectBuffer);
  GraphicsAPI::getDeviceContext().setResourcesNull();
}

}