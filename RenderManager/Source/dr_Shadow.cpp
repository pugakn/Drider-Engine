#include "dr_Shadow.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_depth_stencil.h>
#include <dr_model.h>
#include <dr_resource_manager.h>

namespace driderSDK {

ShadowPass::ShadowPass() {
}

ShadowPass::~ShadowPass() {
}

void
ShadowPass::init(PassInitData* initData) {
  ShadowInitData* data = static_cast<ShadowInitData*>(initData);
  Device& dv = GraphicsAPI::getDevice();

  m_vsFilename = _T("Shadow_vs.hlsl");
  m_fsFilename = _T("Shadow_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)dv.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(dv.createSamplerState(SSdesc));

  /////////////////////////////////////////////////////////////////////////////
  driderSDK::File file;
  String shaderSource;

  file.Open(_T("ShadowMerge_vs.hlsl"));
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_ShaderVMerge = dr_gfx_unique(dv.createShaderFromMemory(shaderSource.data(),
                                                           shaderSource.size(),
                                                           DR_SHADER_TYPE_FLAG::kVertex));

  shaderSource.clear();

  file.Open(_T("ShadowMerge_ps.hlsl"));
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_ShaderFMerge = dr_gfx_unique(dv.createShaderFromMemory(shaderSource.data(),
                                                           shaderSource.size(),
                                                           DR_SHADER_TYPE_FLAG::kFragment));

  shaderSource.clear();
}

void
ShadowPass::recompileShader() {
  RenderPass::recompileShader();

  if (m_ShaderVMerge != nullptr) {
    m_ShaderVMerge->release();
    m_ShaderVMerge.release();
  }
  if (m_ShaderFMerge != nullptr) {
    m_ShaderFMerge->release();
    m_ShaderFMerge.release();
  }

  Device& device = GraphicsAPI::getDevice();
  
  driderSDK::File file;
  String shaderSource;

  file.Open(_T("ShadowMerge_vs.hlsl"));
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_ShaderVMerge = dr_gfx_unique(device.createShaderFromMemory(shaderSource.data(),
                                                               shaderSource.size(),
                                                               DR_SHADER_TYPE_FLAG::kVertex));

  shaderSource.clear();

  file.Open(_T("ShadowMerge_ps.hlsl"));
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_ShaderFMerge = dr_gfx_unique(device.createShaderFromMemory(shaderSource.data(),
                                                               shaderSource.size(),
                                                               DR_SHADER_TYPE_FLAG::kFragment));

  shaderSource.clear();
}

void
ShadowPass::draw(PassDrawData* drawData) {
  ShadowDrawData* data = static_cast<ShadowDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  data->OutRt->getTexture(0).setTextureNull(dc);
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
    CB.World = modelPair.world;
    CB.WVP = modelPair.world * (data->shadowCam->getVP());

    std::memset(&CB.Bones[0].data[0], 0.0f, sizeof(CB.Bones));
    auto Bones = modelPair.bones;
    if (Bones != nullptr) {
      Int32 maxBones = modelPair.bones->size();
      for (SizeT index = 0; index < maxBones; ++index) {
        CB.Bones[index] = (*modelPair.bones)[index];
      }
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

  OutRt->getTexture(0).setTextureNull(dc);
  OutRt->setRTNull(dc);
  OutRt->set(dc, *dsOptions);

  m_ShaderVMerge->set(dc);
  m_ShaderFMerge->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_RTShadowDummy[0]->getTexture(0).set(dc, 0);
  m_RTShadowDummy[1]->getTexture(0).set(dc, 1);
  m_RTShadowDummy[2]->getTexture(0).set(dc, 2);
  m_RTShadowDummy[3]->getTexture(0).set(dc, 3);

  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  OutRt->clear(dc, clearColor);
  dsOptions->clear(dc, 1, 0);

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