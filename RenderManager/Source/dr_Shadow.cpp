#include "dr_Shadow.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_depth_stencil.h>
#include <dr_model.h>
#include <dr_resource_manager.h>
#include <dr_texture_core.h>

namespace driderSDK {

ShadowPass::ShadowPass() {
}

ShadowPass::~ShadowPass() {
}

void
ShadowPass::init(PassInitData* initData) {
  ShadowInitData* data = static_cast<ShadowInitData*>(initData);
  Device& dc = GraphicsAPI::getDevice();

  m_vsFilename = _T("Shadow_vs.hlsl");
  m_fsFilename = _T("Shadow_ps.hlsl");
  m_csFilename = _T("ShadowMerge_cs.hlsl");

  changeSize(data->RTWidht, data->RTHeight);
  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)dc.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(dc.createSamplerState(SSdesc));
}

void
ShadowPass::changeSize(SizeT Width, SizeT Height) {
  RTWidht = Width;
  RTHeight = Height;
}

void
ShadowPass::recompileShader(String vsPreText,
                            String psPreText,
                            String csPreText) {
  String preComputeData = "";
  preComputeData += "#define TXWIDTH " + StringUtils::toString(RTWidht) + "\n";
  preComputeData += "#define TXHEIGHT " + StringUtils::toString(RTHeight) + "\n";

  RenderPass::recompileShader("", "", preComputeData);
}

void
ShadowPass::draw(PassDrawData* drawData) {
  ShadowDrawData* data = static_cast<ShadowDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setResourcesNull();
  data->OutRt->setRTNull(dc);
  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  data->OutRt->clear(dc, clearColor);
  data->dsOptions->clear(dc, 1, 0);

  for (auto& modelPair : *data->models) {
    dc.setResourcesNull();
    if (auto material = modelPair.mesh.material.lock()) {
      if (auto AlbedoTex = material->getProperty(_T("Albedo"))) {
        if (auto GA_Tex = AlbedoTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 0);
        }
      }
    }
    CB.World = modelPair.world;
    CB.WVP = modelPair.world * (data->shadowCam->getVP());

    std::memset(&CB.Bones[0].data[0], 0.0f, sizeof(CB.Bones));
    auto Bones = modelPair.bones;
    if (Bones != nullptr) {
      Int32 maxBones = modelPair.bones->size();
      std::memcpy(&CB.Bones[0],
                  &(*modelPair.bones)[0],
                  sizeof(Matrix4x4) * maxBones);
    }

    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }
}

void
ShadowPass::merge(std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy,
                  GFXShared<DepthStencil> dsOptions,
                  GFXShared<RenderTarget> OutRt) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setResourcesNull();
  OutRt->setRTNull(dc);

  m_computeShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  m_RTShadowDummy[0]->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[1]->getTexture(0).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[2]->getTexture(0).set(dc, 2, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[3]->getTexture(0).set(dc, 3, DR_SHADER_TYPE_FLAG::kCompute);

  OutRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);

  DrTextureDesc outRTDesc = OutRt->getDescriptor();
  dc.dispatch(outRTDesc.width / 8, outRTDesc.height / 4, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}