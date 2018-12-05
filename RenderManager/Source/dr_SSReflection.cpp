#include "dr_SSReflection.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_model.h>
#include <dr_texture.h>
#include <dr_depth_stencil.h>
#include <dr_camera_manager.h>

namespace driderSDK {

SSReflectionsPass::SSReflectionsPass() {
}

SSReflectionsPass::~SSReflectionsPass() {
}

void
SSReflectionsPass::init(PassInitData* initData) {
  SSReflectionsInitData* data = static_cast<SSReflectionsInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  //////////Compile Draw Shader//////////
  m_csFilename = _T("Resources\\Shaders\\SSReflection_cs.hlsl");
  recompileShader();

  DrBufferDesc bdesc;

  //////////Create Draw ConstantBuffer//////////
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBufferDraw);
  m_CBDraw = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  //SamplerState
  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kBorder;
  SSdesc.addressV = SSdesc.addressU;
  SSdesc.addressW = SSdesc.addressV;

  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));

  m_ComputeWidthDivisions = 8;
  m_ComputeHeightDivisions = 4;
}

void
SSReflectionsPass::draw(PassDrawData* drawData) {
  SSReflectionsDrawData* data = static_cast<SSReflectionsDrawData*>(drawData);
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

  m_CBDrawData.ViewportDimensions.x = m_RTWidth;
  m_CBDrawData.ViewportDimensions.y = m_RTHeight;

  m_CBDrawData.EyePosition = data->ActiveCam->getPosition();
  
  m_CBDrawData.CameraInfo.x  = data->ActiveCam->getViewportWidth() /
                               data->ActiveCam->getViewportHeight();
  m_CBDrawData.CameraInfo.y  = data->ActiveCam->getFOV();
  m_CBDrawData.CameraInfo.z  = data->ActiveCam->getNearPlane();
  m_CBDrawData.CameraInfo.w  = data->ActiveCam->getFarPlane();

  m_CBDrawData.CameraVP = data->ActiveCam->getVP();

  m_CBDrawData.ThreadsInfo.x = m_ComputeWidthBlocks;
  m_CBDrawData.ThreadsInfo.y = m_ComputeHeightBlocks;
  m_CBDrawData.ThreadsInfo.z = Math::ceil(m_RTWidth / ((float)RM_TILE_LIGHTS_SZ));
  m_CBDrawData.ThreadsInfo.w = Math::ceil(m_RTHeight / ((float)RM_TILE_LIGHTS_SZ));

  m_CBDraw->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_CBDrawData));
  m_CBDraw->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  data->GbufferRT->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);        //Position, linear depth
  data->GbufferRT->getTexture(1).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, true);        //Normal, CoC
  data->ColorRT->getTexture(0).set(dc, 2, DR_SHADER_TYPE_FLAG::kCompute, true);        //Normal, CoC

  data->OutRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, false);

  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}