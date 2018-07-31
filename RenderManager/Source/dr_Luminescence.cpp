#include "dr_Luminescence.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_depth_stencil.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_camera_manager.h>

namespace driderSDK {

LuminescencePass::LuminescencePass() {
}

LuminescencePass::~LuminescencePass() {
}

void
LuminescencePass::init(PassInitData* initData) {
  LuminescenceInitData* data = static_cast<LuminescenceInitData*>(initData);
  Device& dc = GraphicsAPI::getDevice();

  m_csFilename = _T("Resources\\Shaders\\luminescence_cs.hlsl");
  
  recompileShader();

  m_ComputeWidthDivisions = 8;
  m_ComputeHeightDivisions = 4;

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)dc.createBuffer(bdesc));

  bdesc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
  bdesc.sizeInBytes = sizeof(float) *
                      (data->RTWidth / m_ComputeWidthDivisions) *
                      (data->RTHeight / m_ComputeHeightDivisions);;
  bdesc.stride = sizeof(float);
  m_resultBuffer = dr_gfx_unique((StructureBuffer*)dc.createBuffer(bdesc));

  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;
  m_samplerState = dr_gfx_unique(dc.createSamplerState(SSdesc));
}

void
LuminescencePass::draw(PassDrawData* drawData) {
  LuminescenceDrawData* data = static_cast<LuminescenceDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_computeShader->set(dc);

  DrTextureDesc outRTDesc = data->InTexture->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = m_RTWidth / m_ComputeWidthDivisions;
  m_ComputeHeightBlocks = m_RTHeight / m_ComputeHeightDivisions;

  m_ComputeTotalBlocks = m_ComputeWidthBlocks * m_ComputeHeightBlocks;

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  data->InTexture->set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);

  m_resultBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  CB.fViewportDimensions.x = m_RTWidth;
  CB.fViewportDimensions.y = m_RTHeight;
  CB.fViewportDimensions.z = data->LuminiscenceDelta;

  CB.threadsInfo.x = m_ComputeWidthBlocks;
  CB.threadsInfo.y = m_ComputeHeightBlocks;
  CB.threadsInfo.z = m_ComputeWidthDivisions;
  CB.threadsInfo.w = m_ComputeHeightDivisions;

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);
  //dc.dispatch(1, 1, 1);

  *data->resultBuffer = m_resultBuffer.get();

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}