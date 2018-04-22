#include "dr_PostProcessing.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>

namespace driderSDK {

PostProcessingPass::PostProcessingPass() {
}

PostProcessingPass::~PostProcessingPass() {
}

void
PostProcessingPass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("PostProcessing_vs.hlsl");
  m_fsFilename = _T("PostProcessing_ps.hlsl");

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
PostProcessingPass::draw(PassDrawData* drawData) {
  PostProcessingDrawData* data = static_cast<PostProcessingDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  GraphicsAPI::getBackBufferRT().set(dc, GraphicsAPI::getDepthStencil());

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc,DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  CB.EyePosition = data->activeCam->getPosition();
  CB.EyePosition.w = data->ActiveLights;
  CB.DirLight = data->DirLight;
  for (int lighIndex = 0; lighIndex < 128; ++lighIndex) {
    CB.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
    CB.LightColor[lighIndex] = (*data->Lights)[lighIndex].m_vec4Color;
  }

  CB.View = data->activeCam->getView();
  CB.View.inverse();
  CB.Projection = data->activeCam->getProjection();
  CB.Projection.inverse();
  CB.VP = data->activeCam->getVP();
  CB.VP.inverse();
  for (SizeT i = 0; i < 4; ++i) {
    CB.ShadowVP[i] = (*data->ShadowCam)[i]->getVP();
    CB.ShadowSliptDepth[i] = data->shadowDepths[i];
  }

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->Gbuffer1RT->getTexture(0).set(dc, 0); //Albedo
  data->Gbuffer1RT->getTexture(1).set(dc, 1); //Position
  data->Gbuffer1RT->getTexture(2).set(dc, 2); //Normal
  data->Gbuffer1RT->getTexture(3).set(dc, 3); //Emissivve
  data->Gbuffer1RT->getTexture(4).set(dc, 4); //Metallic
  data->Gbuffer1RT->getTexture(5).set(dc, 5); //Roughness
  data->SSAORT->getTexture(0).set(dc, 6); //SSAO
  data->ShadowRT->getTexture(0).set(dc, 7); //Shadow

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