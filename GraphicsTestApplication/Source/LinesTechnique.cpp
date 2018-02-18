#include "LinesTechnique.h"
#include <dr_camera.h>
#include <dr_device.h>
#include <dr_file.h>
#include <dr_string_utils.h>
#include <dr_shader.h>

namespace driderSDK {

LinesTechnique::LinesTechnique(Camera* _camera, GameObject* _gameObject) 
  : m_camera(_camera),
    m_gameObject(_gameObject)
{}

void
LinesTechnique::compile(Device& device) {
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
  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
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
  auto world = Matrix4x4(Math::FORCE_INIT::kIdentity);
  m_cBuffer.World = world;
  m_cBuffer.WVP = world * m_camera->getVP();
  return reinterpret_cast<UInt8*>(&m_cBuffer);
}
}