#include "dr_GBuffer1.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_model.h>
#include <dr_texture_core.h>

namespace driderSDK {

GBufferPass::GBufferPass() {
}

GBufferPass::~GBufferPass() {
}

void
GBufferPass::init(PassInitData* initData) {
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Resources\\Shaders\\GBuffer1_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\GBuffer1_ps.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.comparisonFunc = DR_COMPARISON_FUNC::kGREATER;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));
}

void
GBufferPass::draw(PassDrawData* drawData) {
  GBufferDrawData* data = static_cast<GBufferDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  data->OutRt->set(dc, *data->dsOptions);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, 0, DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  CB.CameraInfo.x  = static_cast<float>(data->activeCam->getViewportWidth() /
                                        data->activeCam->getViewportHeight());
  CB.CameraInfo.y  = data->activeCam->getFOV();
  CB.CameraInfo.z  = data->activeCam->getNearPlane();
  CB.CameraInfo.w  = data->activeCam->getFarPlane();
  
  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);
  
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  
  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

  //data->OutRt->clear(dc, clearColor);
  data->dsOptions->clear(dc, 1, 0);

  //Skybox
  if (false) {
    dc.setResourcesNull();

    data->cubeMapTex->textureGFX->set(dc, 0);  //Cubemaps
    //cubeMapTex->set(dc, 0);

    //CB.World = data->models->worlds[modelPair.worldID];
    //CB.WorldView = CB.World * data->activeCam->getView();
    //CB.WVP = CB.World * data->activeCam->getVP();

    std::memset(&CB.Bones[0].data[0], 0, sizeof(CB.Bones));

    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
    m_constantBuffer->set(dc);

    //skysphere.get()->meshes.back().vertexBuffer->set(dc);
    //skysphere.get()->meshes.back().indexBuffer->set(dc);
    data->skysphere->meshes.back().vertexBuffer->set(dc);
    data->skysphere->meshes.back().indexBuffer->set(dc);

    dc.draw(data->skysphere->meshes.back().indices.size(), 0, 0);
  }
  data->dsOptions->clear(dc, 1, 0);

  for (auto& modelPair : data->models->commands) {
    dc.setResourcesNull();
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
      auto EmisivityTex = material->getProperty(_T("Emisivity"));
      if (EmisivityTex != nullptr) {
        if (auto GA_Tex = EmisivityTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 2);
        }
      }
      auto MetallicTex = material->getProperty(_T("Metallic"));
      if (MetallicTex != nullptr) {
        if (auto GA_Tex = MetallicTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 3);
        }
      }
      auto RoughnessTex = material->getProperty(_T("Roughness"));
      if (RoughnessTex != nullptr) {
        if (auto GA_Tex = RoughnessTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 4);
        }
      }
    }

    CB.World = data->models->worlds[modelPair.worldID];
    CB.WorldView = CB.World * data->activeCam->getView();
    CB.WVP = CB.World * data->activeCam->getVP();

    std::memset(&CB.Bones[0].data[0], 0, sizeof(CB.Bones));
    std::memset(&CB.ConstantFlags, 0, sizeof(CB.ConstantFlags));
    //Using bones
    if (CB.ConstantFlags[0] = modelPair.bonesID != -1) {
      auto& Bones = data->models->bonesTransforms[modelPair.bonesID];
      Int32 maxBones = Bones.size();
      if (maxBones)
      {
        std::memcpy(&CB.Bones[0],
                    &(Bones)[0],
                    sizeof(Matrix4x4) * maxBones);
      }
    }
  
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
    m_constantBuffer->set(dc);

    modelPair.mesh.vertexBuffer->set(dc);
    modelPair.mesh.indexBuffer->set(dc);

    dc.draw(modelPair.mesh.indicesCount, 0, 0);
  }


  data->OutRt->setRTNull(dc);
  dc.setUAVsNull();
  dc.setResourcesNull();
}
/*
void
GBufferPass::exit() {
  m_inputLayout->release();
  m_constantBuffer->release();

  m_vertexShader->release();
  m_fragmentShader->release();
}
s*/

}