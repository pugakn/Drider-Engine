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
#include <dr_material.h>

namespace driderSDK {

GBufferPass::GBufferPass() {
}

GBufferPass::~GBufferPass() {
}

void
GBufferPass::init(PassInitData* initData) {
  driderSDK::File file;

  file.Open(_T("mesh.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("GBuffer_ps.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  Device& device = GraphicsAPI::getDevice();

  m_vertexShader = device.createShaderFromMemory(vsSource.data(),
                                                 vsSource.size(),
                                                 DR_SHADER_TYPE_FLAG::kVertex);

  m_fragmentShader = device.createShaderFromMemory(fsSource.data,
                                                   fsSource.size(),
                                                   DR_SHADER_TYPE_FLAG::kFragment);

  std::vector<DrInputElementDesc> idesc = m_vertexShader->reflect();

  m_inputLayout = device.createInputLayout(idesc, *m_vertexShader->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
GBufferPass::draw(PassDrawData* drawData) {
  GBufferDrawData* data = static_cast<GBufferDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);

  m_inputLayout->set(dc);

  Matrix4x4 VP = (*data->activeCam).getVP();

  std::vector<std::pair<Matrix4x4, RenderMesh>>* models;

  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  m_constantBuffer->set(dc);

  //dc.setRenderTarget(*data->OutRt, *data->dsOptions);
  
  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.second.material.lock()) {
      material->set();
    }

    //m_constantBuffer->WVP = VP * modelPair.first;
    //m_constantBuffer->World = modelPair.first;

    modelPair.second.vertexBuffer->set(dc);
    modelPair.second.indexBuffer->set(dc);

    dc.draw(modelPair.second.indicesCount, 0, 0);
  }
}

}