#include "dr_HorBlur.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>

namespace driderSDK {

HorBlurPass::HorBlurPass() {
}

HorBlurPass::~HorBlurPass() {
}

void
HorBlurPass::init(PassInitData* initData) {
  HorBlurInitData* data = static_cast<HorBlurInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("HorBlur_vs.hlsl");
  m_fsFilename = _T("HorBlur_ps.hlsl");

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
HorBlurPass::draw(PassDrawData* drawData) {
  HorBlurDrawData* data = static_cast<HorBlurDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->setRTNull(dc);
  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  CB.fViewportDimensions.x = data->viewportDimensionX;
  CB.fViewportDimensions.y = data->viewportDimensionY;

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  data->InRt->getTexture(0).set(dc, 0);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  data->OutRt->clear(dc, clearColor);
  (data->dsOptions)->clear(dc, 1, 0);

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