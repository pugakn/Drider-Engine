#include "dr_Shadow.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_model.h>

namespace driderSDK {

ShadowPass::ShadowPass() {
}

ShadowPass::~ShadowPass() {
}

void
ShadowPass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Shadow_vs.hlsl");
  m_fsFilename = _T("Shadow_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));
}

void
ShadowPass::draw(PassDrawData* drawData) {
  ShadowDrawData* data = static_cast<ShadowDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  if (data->subFrustra < 1) {
    data->OutRt->clear(dc, clearColor);
    data->dsOptions->clear(dc, 1, 0);
  }

  CB.DirectionalLight = Vector4D(data->DirectionalLight, data->subFrustra);

  for (auto& modelPair : *data->models) {
    Matrix4x4 worldTranspose = modelPair.world;
    CB.World = worldTranspose.transpose();
    CB.World = modelPair.world;
    Matrix4x4 newViewProjection = data->activeCam->getVP();
    CB.WVP = modelPair.world * (data->activeCam->getVP());

    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }
}

}