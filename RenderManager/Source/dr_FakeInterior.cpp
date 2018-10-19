#include "dr_FakeInterior.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_model.h>
#include <dr_texture_core.h>

namespace driderSDK {

FakeInteriorPass::FakeInteriorPass() {
}

FakeInteriorPass::~FakeInteriorPass() {
}

void
FakeInteriorPass::init(PassInitData* initData) {
  FakeInteriorInitData* data = static_cast<FakeInteriorInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_vsFilename = _T("Resources\\Shaders\\FakeIntetior_vs.hlsl");
  m_fsFilename = _T("Resources\\Shaders\\FakeInterior_ps.hlsl");

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
  SSdesc.addressV = SSdesc.addressU;
  SSdesc.addressW = SSdesc.addressV;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));
}

void
FakeInteriorPass::draw(PassDrawData* drawData) {
  FakeInteriorDrawData* data = static_cast<FakeInteriorDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  data->OutRt->set(dc, *data->dsOptions);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout->set(dc);

  CB.CameraPosition = Vector4D(data->activeCam->getPosition(), 1.0f);
  
  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc);
  
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
  
  const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  data->OutRt->set(dc, *data->dsOptions);
  data->dsOptions->clear(dc, 1, 0);

  for (auto& modelPair : data->models->commands) {
    dc.setResourcesNull();
    if (auto material = modelPair.mesh.material.lock()) {
      auto FrontTex = material->getProperty(_T("WallFront"));
      if (FrontTex != nullptr) {
        if (auto GA_Tex = FrontTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 0);
        }
      }
      auto LeftTex = material->getProperty(_T("WallLeft"));
      if (LeftTex != nullptr) {
        if (auto GA_Tex = LeftTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 1);
        }
      }
      auto RightTex = material->getProperty(_T("WallRight"));
      if (RightTex != nullptr) {
        if (auto GA_Tex = RightTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 2);
        }
      }
      auto UpTex = material->getProperty(_T("Ceil"));
      if (UpTex != nullptr) {
        if (auto GA_Tex = UpTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 3);
        }
      }
      auto DownTex = material->getProperty(_T("Floor"));
      if (DownTex != nullptr) {
        if (auto GA_Tex = DownTex->texture.lock()) {
          GA_Tex->textureGFX->set(dc, 4);
        }
      }
    }

    CB.World = data->models->worlds[modelPair.worldID];
    CB.WVP = CB.World * data->activeCam->getVP();

    CB.vertice[0] = Vector4D(-50.00000,    0.000000,  50.00000, 1.0f);
    CB.vertice[1] = Vector4D( 50.00000,    0.000000,  50.00000, 1.0f);
    CB.vertice[2] = Vector4D(-50.00000,    0.000000, -50.00000, 1.0f);
    CB.vertice[3] = Vector4D( 50.00000,    0.000000, -50.00000, 1.0f);
    CB.vertice[4] = Vector4D(-50.00000, -100.000000,  50.00000, 1.0f);
    CB.vertice[5] = Vector4D( 50.00000, -100.000000,  50.00000, 1.0f);
    CB.vertice[6] = Vector4D(-50.00000, -100.000000, -50.00000, 1.0f);
    CB.vertice[7] = Vector4D( 50.00000, -100.000000, -50.00000, 1.0f);

    CB.vertice[4].y -= data->depthOffset;
    CB.vertice[5].y -= data->depthOffset;
    CB.vertice[6].y -= data->depthOffset;
    CB.vertice[7].y -= data->depthOffset;

    for (Int32 i = 0; i < 8; ++i) {
      //CB.vertice[i] = CB.World * CB.vertice[i];
      CB.vertice[i] = CB.vertice[i] * CB.World;
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
FakeInteriorPass::exit() {
  m_inputLayout->release();
  m_constantBuffer->release();

  m_vertexShader->release();
  m_fragmentShader->release();
}
s*/

}