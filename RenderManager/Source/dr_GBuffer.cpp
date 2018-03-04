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

  file.Open(_T("vs.hlsl"));
  String vsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  file.Open(_T("fs.hlsl"));
  String fsSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  Device& device = GraphicsAPI::getDevice();

  m_vertexShader = device.createShaderFromMemory(vsSource.data(),
                                                 vsSource.size(),
                                                 DR_SHADER_TYPE_FLAG::kVertex);

  m_fragmentShader = device.createShaderFromMemory(fsSource.data(),
                                                   fsSource.size(),
                                                   DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout = device.createInputLayout(Vertex::getInputDesc(), 
                                           *m_fragmentShader->m_shaderBytecode);

  DrBufferDesc bdesc;

  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constantBuffer = (ConstantBuffer*)device.createBuffer(bdesc);
}

void
GBufferPass::draw(PassDrawData* drawData) {
  GBufferDrawData* data = static_cast<GBufferDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();
  
  Matrix4x4 wvp;
  
  /*
  auto& boneTransforms = animator.getTransforms();

  for (SizeT i = 0; i < boneTransforms.size(); ++i) {
    constBuff.Bones[i] = boneTransforms[i];
  }
  CB.World = transform.getMatrix();
  CB.WVP = wvp;
  */
  
  m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_constantBuffer));
  
  m_constantBuffer->set(dc);
  
  m_inputLayout->set(dc);
  
  m_vertexShader->set(dc);
  m_fragmentShader->set(dc);
  
  dc.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  //dc.setRenderTarget(*data->OutRt, *data->dsOptions);

  std::vector<std::pair<Matrix4x4, RenderMesh>>* models;
  
  for (auto& modelPair : *data->models) {
    if (auto material = modelPair.second.material.lock()) {
      material->set();
    }

    CB.World = modelPair.first;
    CB.WVP = modelPair.first * data->activeCam->getVP();
  
    m_constantBuffer->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_constantBuffer));
  
    m_constantBuffer->set(dc);

    modelPair.second.vertexBuffer->set(dc);
    modelPair.second.indexBuffer->set(dc);

    dc.draw(modelPair.second.indicesCount, 0, 0);
  }
}

/*
  m_constantBuffer->release();
  m_vertexShader->release();
  m_fragmentShader->release();
*/

}