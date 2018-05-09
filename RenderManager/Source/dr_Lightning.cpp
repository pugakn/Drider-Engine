#include "dr_Lightning.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_depth_stencil.h>

namespace driderSDK {

LightningPass::LightningPass() {
}

LightningPass::~LightningPass() {
}

void
LightningPass::init(PassInitData* initData) {
  LightningInitData* data = static_cast<LightningInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Lightning_vs.hlsl");
  m_fsFilename = _T("Lightning_ps.hlsl");

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
LightningPass::draw(PassDrawData* drawData) {
  LightningDrawData* data = static_cast<LightningDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  //GraphicsAPI::getBackBufferRT().set(dc, GraphicsAPI::getDepthStencil());
  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc,DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  CB.EyePosition = data->activeCam->getPosition();
  CB.EyePosition.w = data->ActiveLights;
  CB.DirLight = data->DirLight;
  for (SizeT lighIndex = 0; lighIndex < 128; ++lighIndex) {
    CB.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
    CB.LightColor[lighIndex] = (*data->Lights)[lighIndex].m_vec4Color;
  }

  CB.View = data->activeCam->getView();
  CB.ViewInverse = CB.View;
  CB.ViewInverse.inverse();

  CB.Projection = data->activeCam->getProjection();
  CB.ProjectionInverse = CB.Projection;
  CB.ProjectionInverse.inverse();

  CB.VP = data->activeCam->getVP();
  CB.VPInverse = CB.VP;
  CB.VPInverse.inverse();

  for (SizeT i = 0; i < 4; ++i) {
    CB.ShadowVP[i] = (*data->ShadowCam)[i]->getVP();
    CB.ShadowSliptDepth.data[i] = data->shadowDepths[i + 1];
  }

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  data->GbufferRT->getTexture(0).set(dc, 0); //Position, linear depth
  data->GbufferRT->getTexture(1).set(dc, 1); //Normal, CoC
  data->GbufferRT->getTexture(2).set(dc, 2); //Albedo, Metallic
  data->GbufferRT->getTexture(3).set(dc, 3); //Emissivve, Roughness
  data->SSAORT->getTexture(0).set(dc, 4);    //SSAO
  data->ShadowRT->getTexture(0).set(dc, 5);  //Shadow

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  data->OutRt->clear(dc, clearColor);
  data->dsOptions->clear(dc, 1, 0);

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