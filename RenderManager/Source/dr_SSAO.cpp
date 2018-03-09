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
#include <dr_resource_manager.h>
#include <dr_model.h>

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

  m_inputLayout = device.createInputLayout(m_vertexShader->reflect(),
                                           *m_vertexShader->m_shaderBytecode);

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_MIP_POINT;
  m_samplerState = device.createSamplerState(SSdesc);
}

void
SSAOPass::draw(PassDrawData* drawData) {
  SSAODrawData* data = static_cast<SSAODrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  GraphicsAPI::getBackBufferRT().set(dc, GraphicsAPI::getDepthStencil());

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->GbufferRT->getTexture(0).set(dc, 0);
  data->GbufferRT->getTexture(1).set(dc, 1);
  data->GbufferRT->getTexture(2).set(dc, 2);

  auto screenQuadModel = ResourceManager::getReferenceT<Model>(_T("ScreenAlignedQuad.3ds"));
  if (screenQuadModel) {
    for (auto& SAQ : screenQuadModel->meshes) {
      SAQ.vertexBuffer->set(dc);
      SAQ.indexBuffer->set(dc);

      dc.draw(SAQ.indices.size(), 0, 0);
    }
  }

}

}