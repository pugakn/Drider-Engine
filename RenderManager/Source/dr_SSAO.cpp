#include "dr_SSAO.h"
#include <dr_string_utils.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_constant_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_vertex.h>
#include <dr_render_component.h>
#include <dr_device_context.h>
#include <dr_camera.h>

namespace driderSDK {

SSAOPass::SSAOPass() {
}

SSAOPass::~SSAOPass() {
}

void
SSAOPass::init(PassInitData* initData) {
  driderSDK::File file;

  file.Open(_T("SSAO_vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("SSAO_ps.hlsl"));
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
SSAOPass::draw(PassDrawData* drawData) {
  PassDrawData* data = static_cast<PassDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.second.material.lock()) {
      material->set();
    }

    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

    m_constantBuffer->set(dc);

    modelPair.second.vertexBuffer->set(dc);
    modelPair.second.indexBuffer->set(dc);

    dc.draw(modelPair.second.indicesCount, 0, 0);
  }
}

}