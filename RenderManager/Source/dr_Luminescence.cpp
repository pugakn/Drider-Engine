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

  m_csFilename = _T("luminescence_cs.hlsl");
  
  recompileShader();

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = dr_gfx_unique((ConstantBuffer*)dc.createBuffer(bdesc));

  bdesc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
  bdesc.sizeInBytes = sizeof(Vector4D);
  bdesc.stride = sizeof(Vector4D);
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

  m_computeShader->set(dc);

  data->InTexture->set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  CB.LuminiscenceDelta = data->LuminiscenceDelta;
  CB.TextureWidth = static_cast<float>(data->InTexture->getDescriptor().width);
  CB.TextureHeight = static_cast<float>(data->InTexture->getDescriptor().height);

  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&CB));
  m_constantBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  m_resultBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  dc.dispatch(1, 1, 1);

  *data->resultBuffer = m_resultBuffer.get();

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}