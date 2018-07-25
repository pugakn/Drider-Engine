#include "dr_Shadow.h"
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device.h>
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_depth_stencil.h>
#include <dr_model.h>
#include <dr_resource_manager.h>
#include <dr_texture_core.h>
#include <dr_file.h>


namespace driderSDK {

ShadowPass::ShadowPass() {
}

ShadowPass::~ShadowPass() {
}

void
ShadowPass::init(PassInitData* initData) {
  ShadowInitData* data = static_cast<ShadowInitData*>(initData);
  Device& dc = GraphicsAPI::getDevice();

  m_vsFilename = _T("Resources\\Shaders\\Shadow_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\Shadow_ps.hlsl");
  m_csFilename = _T("Resources\\Shaders\\ShadowMerge_cs.hlsl");
  m_csFilenameShadowApply = _T("Resources\\Shaders\\ShadowApply_cs.hlsl");

  changeSize(data->RTWidht, data->RTHeight);
  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;

  bdesc.sizeInBytes = sizeof(CBuffer1);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)dc.createBuffer(bdesc));

  bdesc.sizeInBytes = sizeof(CBuffer2);
  m_constantBufferSSShadow = dr_gfx_unique((ConstantBuffer*)dc.createBuffer(bdesc));

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
  Device& dc = GraphicsAPI::getDevice();

  String preComputeData = "";
  preComputeData += "#define TXWIDTH " + StringUtils::toString(RTWidht) + "\n";
  preComputeData += "#define TXHEIGHT " + StringUtils::toString(RTHeight) + "\n";

  RenderPass::recompileShader("", "", preComputeData);

  File file;
  String shaderSrc;

  file.Open(m_csFilenameShadowApply);
  shaderSrc = csPreText + StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_computeShaderApply = dr_gfx_unique(dc.createShaderFromMemory(shaderSrc.data(),
                                  shaderSrc.size(),
                                  DR_SHADER_TYPE_FLAG::kCompute));

  shaderSrc.clear();
}

void
ShadowPass::draw(PassDrawData* drawData) {
  ShadowDrawData* data = static_cast<ShadowDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  data->OutRt->set(dc, *data->dsOptions);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CascadeCB));
  m_constantBuffer->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  data->OutRt->clear(dc, clearColor);
  data->dsOptions->clear(dc, 1, 0);

  for (auto& modelPair : data->models->commands) {
    dc.setResourcesNull();
    if (auto material = modelPair.mesh.material.lock()) {
      if (auto AlbedoTex = material->getProperty(_T("Albedo"))) {
        if (auto GA_Tex = AlbedoTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 0);
        }
      }
    }
    CascadeCB.WVP = data->models->worlds[modelPair.worldID] * (data->shadowCam->getVP());

    std::memset(&CascadeCB.Bones[0].data[0], 0.0f, sizeof(CascadeCB.Bones));
    if (modelPair.bonesID != -1) {
      auto& Bones = data->models->bonesTransforms[modelPair.bonesID];
      Int32 maxBones = Bones.size();
      std::memcpy(&CascadeCB.Bones[0],
                  &(Bones)[0],
                  sizeof(Matrix4x4) * maxBones);
    }

    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CascadeCB));
    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }

  dc.setUAVsNull();
  dc.setResourcesNull();
}

void
ShadowPass::merge(std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy,
                  GFXShared<RenderTarget> CompressedShadowsOutRt) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_computeShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  m_RTShadowDummy[0]->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[1]->getTexture(0).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[2]->getTexture(0).set(dc, 2, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTShadowDummy[3]->getTexture(0).set(dc, 3, DR_SHADER_TYPE_FLAG::kCompute);

  CompressedShadowsOutRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);

  DrTextureDesc outRTDesc = CompressedShadowsOutRt->getDescriptor();

  dc.dispatch(outRTDesc.width / 8, outRTDesc.height / 4, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

void
ShadowPass::apply(PassDrawData* drawData,
                  GFXShared<RenderTarget> PositionDepthRt,
                  GFXShared<RenderTarget> CompressedShadowsOutRt,
                  GFXShared<RenderTarget> ResultShadowsRt) {
  ShadowDrawData* data = static_cast<ShadowDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_computeShaderApply->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  PositionDepthRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);
  CompressedShadowsOutRt->getTexture(0).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, true);

  ResultShadowsRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);

  DrTextureDesc outRTDesc = ResultShadowsRt->getDescriptor();

  for (SizeT i = 0; i < data->ActivatedShadowCascades; ++i) {
    ShadowCB.ShadowVP[i] = (*data->ShadowCameras)[i]->getVP();
    ShadowCB.ShadowSplitDepth[i] = data->ShadowSliptDepths[i + 1];
  }
  ShadowCB.ShadowSizesProportion = data->ShadowSizesProportion;

  ShadowCB.ShadowInfo[0] = data->ActivatedShadowCascades;
  ShadowCB.ShadowInfo[1] = data->ShadowMapTextureSize;
  ShadowCB.ShadowInfo[2] = data->LerpBetweenShadowCascade;

  ShadowCB.fViewportDimensions.x = outRTDesc.width;
  ShadowCB.fViewportDimensions.y = outRTDesc.height;

  m_constantBufferSSShadow->updateFromBuffer(dc, reinterpret_cast<byte*>(&ShadowCB));
  m_constantBufferSSShadow->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
  dc.dispatch(outRTDesc.width / 8, outRTDesc.height / 4, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}