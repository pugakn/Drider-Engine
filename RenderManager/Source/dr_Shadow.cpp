#include "dr_Shadow.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_depth_stencil.h>
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

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMAXIMUM_ANISOTROPIC;
  SSdesc.maxAnisotropy = 17;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));
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

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->OutRt->getTexture(0).set(dc, 0);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  if (data->shadowIndex < 1) {
    data->OutRt->clear(dc, clearColor);
    data->dsOptions->clear(dc, 1, 0);
  }

  CB.ShadowIndex[0] = 0;
  CB.ShadowIndex[1] = 0;
  CB.ShadowIndex[2] = 0;
  CB.ShadowIndex[3] = 0;

  CB.ShadowIndex[data->shadowIndex] = 1;

  CB.extraInfo.x = data->activeCam->getNearPlane();
  CB.extraInfo.y = data->activeCam->getFarPlane();

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