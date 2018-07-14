#include "dr_SSAO.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_camera.h>

namespace driderSDK {

SSAOPass::SSAOPass() {
}

SSAOPass::~SSAOPass() {
}

void
SSAOPass::init(PassInitData* initData) {
  SSAOInitData* data = static_cast<SSAOInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  m_csFilename = _T("SSAO_cs.hlsl");

  recompileShader();

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));

  m_ComputeWidthDivisions = 8;
  m_ComputeHeightDivisions = 4;
}

void
SSAOPass::draw(PassDrawData* drawData) {
  SSAODrawData* data = static_cast<SSAODrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setResourcesNull();
  data->OutRt->setRTNull(dc);

  m_computeShader->set(dc);

  DrTextureDesc outRTDesc = data->OutRt->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = m_RTWidth / m_ComputeWidthDivisions;
  m_ComputeHeightBlocks = m_RTHeight / m_ComputeHeightDivisions;

  m_ComputeTotalBlocks = m_ComputeWidthBlocks * m_ComputeHeightBlocks;

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  data->InRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);
  data->InRt->getTexture(1).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, true);

  data->OutRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);

  CB.fViewportDimensions.x = m_RTWidth;
  CB.fViewportDimensions.y = m_RTHeight;
  CB.SSAO_Options[0] = data->SampleRadio;
  CB.SSAO_Options[1] = data->Intensity;
  CB.SSAO_Options[2] = data->Scale;
  CB.SSAO_Options[3] = data->Bias;

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
  
  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);

  dc.setUAVsNull();
}

}