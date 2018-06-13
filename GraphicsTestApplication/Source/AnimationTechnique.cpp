#include "AnimationTechnique.h"
#include <dr_camera.h>
#include <dr_device.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_shader.h>
#include <dr_string_utils.h>
#include <dr_vertex.h>

namespace driderSDK {

void 
AnimationTechnique::setBones(const std::vector<Matrix4x4>& bones) {

  Int32 index = 0;
  for (auto& bone : bones) {
    m_cBuffer.Bones[index++] = bone;
  }

}
void
AnimationTechnique::compile() {
  auto& device = GraphicsAPI::getDevice();

  File file;

  file.Open(_T("Resources\\Shaders\\anim_vs.hlsl"));

  String vertexSrc = StringUtils::toString(file.GetAsString(file.Size()));
  
  file.Close();

  file.Open(_T("Resources\\Shaders\\anim_fs.hlsl"));

  String fragmentSrc = StringUtils::toString(file.GetAsString(file.Size()));

  file.Close();
  
  auto vertexShader = device.createShaderFromMemory(vertexSrc.data(), 
                                                    vertexSrc.size(), 
                                                    DR_SHADER_TYPE_FLAG::kVertex);

  auto fragmentShader = device.createShaderFromMemory(fragmentSrc.data(), 
                                                      fragmentSrc.size(), 
                                                      DR_SHADER_TYPE_FLAG::kFragment);

  m_inputLayout = device.createInputLayout(Vertex::getInputDesc(), 
                                           *vertexShader->m_shaderBytecode);

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constBuffer = (ConstantBuffer*)(device.createBuffer(bdesc));

  m_shaders.push_back(vertexShader);
  m_shaders.push_back(fragmentShader);
}

UInt8* 
AnimationTechnique::getConstBufferData() {
  Matrix4x4 world(Math::FORCE_INIT::kIdentity);

  if (m_world) {
    world = *m_world;
  }

  Matrix4x4 vp(Math::FORCE_INIT::kIdentity);

  if (m_camera) {
    vp = m_camera->getVP();
  }

  m_cBuffer.WVP = world * vp;
  m_cBuffer.World = world;

  return reinterpret_cast<UInt8*>(&m_cBuffer);
}
}