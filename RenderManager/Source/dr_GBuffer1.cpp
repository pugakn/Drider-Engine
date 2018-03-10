#include "dr_GBuffer1.h"
#include <dr_device.h>
#include <dr_string_utils.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_constant_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_vertex.h>
#include <dr_render_component.h>
#include <dr_device_context.h>
#include <dr_camera.h>
#include <dr_material.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_texture_core.h>

namespace driderSDK {

GBuffer1Pass::GBuffer1Pass() {
}

GBuffer1Pass::~GBuffer1Pass() {
}

void
GBuffer1Pass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("GBuffer1_vs.hlsl");
  m_fsFilename = _T("GBuffer1_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
GBuffer1Pass::draw(PassDrawData* drawData) {
  GBuffer1DrawData* data = static_cast<GBuffer1DrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->set(dc, *data->dsOptions);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);
  
  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  
  m_constantBuffer->set(dc);
  
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  
  const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  data->OutRt->clear(dc, clearColor);
  (data->dsOptions)->clear(dc, 1, 0);

  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.mesh.material.lock()) {
      auto AlbedoTex = material->getProperty(_T("Albedo"));
      if (AlbedoTex != nullptr) {
        if (auto GA_Tex = AlbedoTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 0);
        }
      }
      auto NormalTex = material->getProperty(_T("Normal"));
      if (NormalTex != nullptr) {
        if (auto GA_Tex = NormalTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 1);
        }
      }
      auto EmissiveTex = material->getProperty(_T("Emisivity"));
      if (EmissiveTex != nullptr) {
        if (auto GA_Tex = EmissiveTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 2);
        }
      }
    }

    CB.World = modelPair.world;
    CB.WVP = modelPair.world * data->activeCam->getVP();
  
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  
    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }
}
/*
void
GBuffer1Pass::exit() {
  m_inputLayout->release();
  m_constantBuffer->release();

  m_vertexShader->release();
  m_fragmentShader->release();
}
s*/

}