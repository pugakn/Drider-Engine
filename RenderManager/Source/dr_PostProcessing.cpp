#include "dr_PostProcessing.h"
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

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_MIP_POINT;
  m_samplerState = device.createSamplerState(SSdesc);
}

void
PostProcessingPass::draw(PassDrawData* drawData) {
  PostProcessingDrawData* data = static_cast<PostProcessingDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  GraphicsAPI::getBackBufferRT().set(dc, GraphicsAPI::getDepthStencil());

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  data->Gbuffer1RT->getTexture(0).set(dc, 0); //Albedo
  data->Gbuffer1RT->getTexture(1).set(dc, 1); //Position
  data->Gbuffer1RT->getTexture(2).set(dc, 2); //Normal
  data->Gbuffer1RT->getTexture(3).set(dc, 3); //Emissivve
  data->Gbuffer2RT->getTexture(0).set(dc, 4); //Metallic
  data->Gbuffer2RT->getTexture(1).set(dc, 5); //Roughness
  data->Gbuffer2RT->getTexture(2).set(dc, 6); //SSAO

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