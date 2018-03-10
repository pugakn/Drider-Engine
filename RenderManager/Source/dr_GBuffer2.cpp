#include "dr_GBuffer2.h"
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

GBuffer2Pass::GBuffer2Pass() {
}

GBuffer2Pass::~GBuffer2Pass() {
}

void
GBuffer2Pass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("GBuffer2_vs.hlsl");
  m_fsFilename = _T("GBuffer2_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
GBuffer2Pass::draw(PassDrawData* drawData) {
  GBuffer2DrawData* data = static_cast<GBuffer2DrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));

  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  data->InRt->getTexture(1).set(dc, 0);
  data->InRt->getTexture(2).set(dc, 1);

  const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  data->OutRt->clear(dc, clearColor);
  (data->dsOptions)->clear(dc, 1, 0);

  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.mesh.material.lock()) {
      auto MetallicTex = material->getProperty(_T("Metallic"));
      if (MetallicTex != nullptr) {
        if (auto GA_Tex = MetallicTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 2);
        }
      }
      auto RoughTex = material->getProperty(_T("Roughness"));
      if (RoughTex != nullptr) {
        if (auto GA_Tex = RoughTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 3);
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

}