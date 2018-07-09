#include "dr_Luminescence.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
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

  driderSDK::File file;
  String shaderSrc = "";

  file.Open(_T("luminescence_cs.hlsl"));
  shaderSrc += StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_computeShader = dr_gfx_unique(dc.createShaderFromMemory(shaderSrc.data(),
                                                            shaderSrc.size(),
                                                            DR_SHADER_TYPE_FLAG::kCompute));
  shaderSrc.clear();
}

void
LuminescencePass::draw(PassDrawData* drawData) {
  LuminescenceDrawData* data = static_cast<LuminescenceDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  m_computeShader->set(dc);

  m_CB.LuminiscenceDelta = data->LuminiscenceDelta;
  m_CB.TextureWidth = data->InTexture->getDescriptor().width;
  m_CB.TextureHeight = data->InTexture->getDescriptor().height;

  m_constantBuffer->updateFromBuffer(dc,(byte*)&m_CB);
  m_constantBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  data->InTexture->set(dc, 0);

  dc.dispatch(1, 1, 1);

  GraphicsAPI::getDeviceContext().setResourcesNull();
}

}