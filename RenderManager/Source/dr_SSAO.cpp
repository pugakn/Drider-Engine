#include "dr_SSAO.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_camera.h>

namespace driderSDK {

SSAOPass::SSAOPass() {
}

SSAOPass::~SSAOPass() {
}

void
SSAOPass::init(PassInitData* initData) {
  SSAOInitData* data = static_cast<SSAOInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("SSAO_vs.hlsl");
  m_fsFilename = _T("SSAO_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_MIP_LINEAR;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));
}

void
SSAOPass::draw(PassDrawData* drawData) {
  SSAODrawData* data = static_cast<SSAODrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  CB.View = data->activeCam->getView();
  CB.ViewInverse = CB.View;
  CB.ViewInverse.inverse();

  CB.Projection = data->activeCam->getProjection();
  CB.ProjectionInverse = CB.Projection;
  CB.ProjectionInverse.inverse();

  CB.VP = data->activeCam->getVP();
  CB.VPInverse = CB.VP;
  CB.VPInverse.inverse();

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  m_inputLayout->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  data->OutRt->clear(dc, clearColor);
  (data->dsOptions)->clear(dc, 1, 0);

  data->InRt->getTexture(0).set(dc, 0);
  data->InRt->getTexture(1).set(dc, 1);

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