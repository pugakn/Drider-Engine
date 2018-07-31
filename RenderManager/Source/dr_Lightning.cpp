#include "dr_Lightning.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_texture.h>
#include <dr_depth_stencil.h>
#include <dr_camera_manager.h>

namespace driderSDK {

LightningPass::LightningPass() {
}

LightningPass::~LightningPass() {
}

void
LightningPass::init(PassInitData* initData) {
  LightningInitData* data = static_cast<LightningInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_csFilename = _T("Resources\\Shaders\\Lightning_cs.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer1);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer2);
  m_constantBufferTiled = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;

  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));

  m_ComputeWidthDivisions = 8;
  m_ComputeHeightDivisions = 4;

  File file;
  String shaderSrc;

  m_csTiledLightsFilename = _T("Resources\\Shaders\\TileLights_cs.hlsl");

  file.Open(m_csTiledLightsFilename);
  shaderSrc = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_csTiledLights = dr_gfx_unique(device.createShaderFromMemory(shaderSrc.data(),
                                  shaderSrc.size(),
                                  DR_SHADER_TYPE_FLAG::kCompute));

  shaderSrc.clear();

  bdesc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
  bdesc.sizeInBytes = sizeof(Int32) *
                      (data->RTWidth / m_ComputeWidthDivisions) *
                      (data->RTHeight / m_ComputeHeightDivisions);
  bdesc.stride = sizeof(Int32);
  m_sbNumberOfLights = dr_gfx_unique((StructureBuffer*)device.createBuffer(bdesc));

  bdesc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
  bdesc.sizeInBytes = sizeof(Int32) *
                      128 *
                      (data->RTWidth / m_ComputeWidthDivisions) *
                      (data->RTHeight / m_ComputeHeightDivisions);
  bdesc.stride = sizeof(Int32) * 128;
  m_sbLightsIndex = dr_gfx_unique((StructureBuffer*)device.createBuffer(bdesc));
}

void
LightningPass::draw(PassDrawData* drawData) {
  LightningDrawData* data = static_cast<LightningDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_computeShader->set(dc);

  DrTextureDesc outRTDesc = data->OutRt->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = m_RTWidth / m_ComputeWidthDivisions;
  m_ComputeHeightBlocks = m_RTHeight / m_ComputeHeightDivisions;

  m_ComputeTotalBlocks = m_ComputeWidthBlocks * m_ComputeHeightBlocks;

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  CB.fViewportDimensions.x = m_RTWidth;
  CB.fViewportDimensions.y = m_RTHeight;

  CB.EyePosition = data->ActiveCam->getPosition();
  CB.EyePosition.w = data->ActiveLights;

  for (SizeT lighIndex = 0; lighIndex < 128; ++lighIndex) {
    CB.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
    CB.LightColor[lighIndex] = (*data->Lights)[lighIndex].m_vec4Color;
  }

  CB.threadsInfo.x = m_ComputeWidthBlocks;
  CB.threadsInfo.y = m_ComputeHeightBlocks;

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  data->GbufferRT->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);        //Position, linear depth
  data->GbufferRT->getTexture(1).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, true);        //Normal, CoC
  data->GbufferRT->getTexture(2).set(dc, 2, DR_SHADER_TYPE_FLAG::kCompute, true);        //Albedo, Metallic
  data->GbufferRT->getTexture(3).set(dc, 3, DR_SHADER_TYPE_FLAG::kCompute, true);        //Emissivve, Roughness
  data->SSAORT->getTexture(0).set(dc, 4, DR_SHADER_TYPE_FLAG::kCompute, true);           //SSAO
  data->ShadowRT->getTexture(0).set(dc, 5, DR_SHADER_TYPE_FLAG::kCompute, true);         //Shadow
  data->EnviromentCubemap->textureGFX->set(dc, 6, DR_SHADER_TYPE_FLAG::kCompute, true);  //Cubemap
  data->IrradianceCubemap->textureGFX->set(dc, 7, DR_SHADER_TYPE_FLAG::kCompute, true);  //CubemapDiffuse

  m_sbLightsIndex->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
  data->OutRt->getTexture(0).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, false);

  dc.dispatch(outRTDesc.width / 8, outRTDesc.height / 4, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

void
LightningPass::tileLights(PassDrawData* drawData) {
  LightningDrawData* data = static_cast<LightningDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_csTiledLights->set(dc);

  DrTextureDesc outRTDesc = data->OutRt->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = m_RTWidth / m_ComputeWidthDivisions;
  m_ComputeHeightBlocks = m_RTHeight / m_ComputeHeightDivisions;

  m_ComputeTotalBlocks = m_ComputeWidthBlocks * m_ComputeHeightBlocks;

  m_sbLightsIndex->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  CB.fViewportDimensions.x = m_RTWidth;
  CB.fViewportDimensions.y = m_RTHeight;

  CBTiled.CameraUp = Vector4D(CameraManager::getActiveCamera()->getLocalUp(), 0.0f);

  CBTiled.ThreadsGroups.x = m_ComputeWidthBlocks;
  CBTiled.ThreadsGroups.y = m_ComputeHeightBlocks; 

  Matrix4x4 CamVP = CameraManager::getActiveCamera()->getVP();
  CBTiled.VP = CamVP;

  for (SizeT lighIndex = 0; lighIndex < 128; ++lighIndex) {
    CBTiled.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
  }

  m_constantBufferTiled->updateFromBuffer(dc, reinterpret_cast<byte*>(&CBTiled));
  m_constantBufferTiled->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}