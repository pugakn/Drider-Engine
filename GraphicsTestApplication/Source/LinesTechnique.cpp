#include "LinesTechnique.h"
#include <dr_camera.h>
#include <dr_device.h>
#include <dr_file.h>
#include <dr_graphics_api.h>
#include <dr_shader.h>
#include <dr_string_utils.h>

namespace driderSDK {

void
LinesTechnique::compile() {
  
  auto& device = GraphicsAPI::getDevice();

  File file;

  file.Open(_T("lines_vs.hlsl"));

  String vertexSrc = StringUtils::toString(file.GetAsString(file.Size()));
  
  file.Close();

  file.Open(_T("lines_fs.hlsl"));

  String fragmentSrc = StringUtils::toString(file.GetAsString(file.Size()));

  file.Close();
  
  auto vertexShader = device.createShaderFromMemory(vertexSrc.data(), 
                                                    vertexSrc.size(), 
                                                    DR_SHADER_TYPE_FLAG::kVertex);

  auto fragmentShader = device.createShaderFromMemory(fragmentSrc.data(), 
                                                      fragmentSrc.size(), 
                                                      DR_SHADER_TYPE_FLAG::kFragment);

  std::vector<DrInputElementDesc> inputDesc;
  DrInputElementDesc ie;
  ie.format = DR_FORMAT::kR32G32B32A32_FLOAT;
  ie.offset = 0;
  ie.semanticName = "POSITION";
  inputDesc.push_back(ie);

  m_inputLayout = device.createInputLayout(inputDesc, 
                                           *vertexShader->m_shaderBytecode);

  DrBufferDesc bdesc;
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBuffer);
  m_constBuffer = (ConstantBuffer*)(device.createBuffer(bdesc));

  m_shaders.push_back(vertexShader);
  m_shaders.push_back(fragmentShader);
}

UInt8* 
LinesTechnique::getConstBufferData() {
  
  Matrix4x4 world(Math::FORCE_INIT::kIdentity);

  Matrix4x4 vp(Math::FORCE_INIT::kIdentity);

  if (m_camera) {
    vp = m_camera->getVP();
  }

  m_cBuffer.World = world;
  m_cBuffer.WVP = world * vp;

  return reinterpret_cast<UInt8*>(&m_cBuffer);
}
}