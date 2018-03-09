#include "dr_GBuffer.h"
#include <dr_string_utils.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_constant_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_vertex.h>
#include <dr_render_component.h>
#include <dr_device_context.h>
#include <dr_camera.h>
#include <dr_material.h>
#include <dr_resource_manager.h>
#include <dr_model.h>

namespace driderSDK {

GBufferPass::GBufferPass() {
}

GBufferPass::~GBufferPass() {
}

void
GBufferPass::init(PassInitData* initData) {
  driderSDK::File file;

  file.Open(_T("GBuffer_vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("GBuffer_ps.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  Device& device = GraphicsAPI::getDevice();

  m_vertexShader = device.createShaderFromMemory(vsSource.data(),
                                                 vsSource.size(),
                                                 DR_SHADER_TYPE_FLAG::kVertex);

  m_fragmentShader = device.createShaderFromMemory(fsSource.data(),
                                                   fsSource.size(),
                                                   DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout = device.createInputLayout(Vertex::getInputDesc(), 
                                           *m_vertexShader->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
GBufferPass::draw(PassDrawData* drawData) {
  GBufferDrawData* data = static_cast<GBufferDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->set(dc, *data->dsOptions);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);
  
  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  
  m_constantBuffer->set(dc);
  
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  
  const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  data->OutRt->clear(dc, clearColor);
  (data->dsOptions)->clear(dc, 1, 0);

  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.mesh.material.lock()) {
      material->set();
    }

    CB.World = modelPair.world;
    CB.WVP = modelPair.world * data->activeCam->getVP();
  
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  
    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }
}

/*
  m_constantBuffer->release();
  m_vertexShader->release();
  m_fragmentShader->release();
*/

}