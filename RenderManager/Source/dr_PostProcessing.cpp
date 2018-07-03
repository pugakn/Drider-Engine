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
  PostProcessingInitData* data = static_cast<PostProcessingInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("PostProcessing_vs.hlsl");
  m_fsFilename = _T("PostProcessing_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
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

  dc.setResourcesNull();
  GraphicsAPI::getBackBufferRT().setRTNull(dc);
  GraphicsAPI::getBackBufferRT().set(dc, GraphicsAPI::getDepthStencil());

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc,DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  CB.CameraInfo.x = data->activeCam->getViewportWidth() /
                    data->activeCam->getViewportHeight();
  CB.CameraInfo.y = data->activeCam->getFOV();
  CB.CameraInfo.z = data->activeCam->getNearPlane();
  CB.CameraInfo.w = data->activeCam->getFarPlane();

  CB.CA_CoC_V.x = data->ChromaticAberrationStrenght;
  CB.CA_CoC_V.y = data->CoCFocusDistance;
  CB.CA_CoC_V.z = data->CoCFocusRange;
  CB.CA_CoC_V.w = data->VignetteScale;

  CB.VignetteOptions = Vector4D(data->VignetteConcentration, data->VignetteRad);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->PositionDepthRT->getTexture(0).set(dc, 0);
  data->ColorRT->getTexture(0).set(dc, 1);
  data->ColorBlurRT->getTexture(0).set(dc, 2);
  data->Gbuffer->getTexture(1).set(dc, 3);
  data->luminescenceBuffer->set(dc, DR_SHADER_TYPE_FLAG::kFragment, 4);

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