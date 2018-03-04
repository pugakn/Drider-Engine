#include "dr_SSAO.h"
#include <dr_string_utils.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_constant_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_vertex.h>
#include <dr_render_component.h>
#include <dr_device_context.h>
#include <dr_camera.h>

namespace driderSDK {

SSAOPass::SSAOPass() {
}

SSAOPass::~SSAOPass() {
}

void
SSAOPass::init(PassInitData* initData) {
  driderSDK::File file;

  file.Open(_T("SSAO_vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("SSAO_ps.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  Device& device = GraphicsAPI::getDevice();

  m_vertexShader = device.createShaderFromMemory(vsSource.c_str(),
                                                 vsSource.size() + 1,
                                                 DR_SHADER_TYPE_FLAG::kVertex);

  m_fragmentShader = device.createShaderFromMemory(fsSource.c_str(),
                                                   fsSource.size() + 1,
                                                   DR_SHADER_TYPE_FLAG::kFragment);

  std::vector<DrInputElementDesc> idesc = m_vertexShader->reflect();

  m_inputLayout = device.createInputLayout(idesc, *m_vertexShader->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
SSAOPass::draw(PassDrawData* drawData) {
  SSAODrawData* data = static_cast<SSAODrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  m_constantBuffer->set(dc);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);
}

}