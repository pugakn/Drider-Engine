#include "dr_GBuffer.h"
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

GBufferPass::GBufferPass() {
}

GBufferPass::~GBufferPass() {
}

template<typename T, typename... Args>
void GBufferPass::init(T t, Args... args) {
  driderSDK::File file;

  file.Open(_T("GBuffer_vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("GBuffer_ps.hlsl"));
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

template<typename T, typename... Args>
void GBufferPass::draw(T t, Args... args) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  m_constantBuffer->set(dc);

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  Matrix4x4 VP = static_cast<Camera*>(data)->getVP();

  std::vector<std::pair<Matrix4x4, RenderMesh>>* models =
    static_cast<std::vector<std::pair<Matrix4x4, RenderMesh>>*>(data);

  for (auto& pAlgo : *models) {
    //m_constantBuffer->WVP = VP * pAlgo.first;
    //m_constantBuffer->World = pAlgo.first;

    pAlgo.second.vertexBuffer->set(dc);
    pAlgo.second.indexBuffer->set(dc);

    dc.draw(pAlgo.second.indicesCount, 0, 0);
  }
}

}