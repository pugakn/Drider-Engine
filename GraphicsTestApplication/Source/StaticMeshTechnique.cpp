#include "StaticMeshTechnique.h"
#include <dr_camera.h>
#include <dr_device.h>
#include <dr_file.h>
#include <dr_graphics_defines.h>
#include <dr_input_layout.h>
#include <dr_gameObject.h>
#include <dr_shader.h>
#include <dr_string_utils.h>

namespace driderSDK {

StaticMeshTechnique::StaticMeshTechnique(Camera* camera_, 
                                         GameObject* gameObject_) 
  : m_camera(camera_),
    m_gameObject(gameObject_) 
{}

void
StaticMeshTechnique::compile(Device& device) {
  
  File file;

  file.Open(_T("mesh.hlsl"));

  String vertexSrc = StringUtils::toString(file.GetAsString(file.Size()));
  
  file.Close();

  file.Open(_T("fs.hlsl"));

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

  ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  ie.offset = 16;
  ie.semanticName = "NORMAL";
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
StaticMeshTechnique::getConstBufferData() {
  
  auto& world = m_gameObject->getWorldTransform();
  m_constBufferObj.WVP = world * m_camera->getVP();
  m_constBufferObj.World = world;

  return reinterpret_cast<UInt8*>(&m_constBufferObj);
}

}
