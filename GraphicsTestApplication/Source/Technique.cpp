#include "Technique.h"
#include <dr_constant_buffer.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_input_layout.h>
#include <dr_shader.h>


namespace driderSDK {

Technique::Technique(Camera* _camera, const Matrix4x4* _world) 
  : m_camera(_camera),
    m_world(_world)
{}

bool
Technique::prepareForDraw() {

  auto& deviceContext = GraphicsAPI::getDeviceContext();

  bool prepared = false;  

  auto constBufferData = getConstBufferData();

  if (m_constBuffer && 
      constBufferData && 
      m_inputLayout && 
      m_shaders.size() > 1) {

    m_constBuffer->updateFromBuffer(deviceContext, constBufferData);
    m_constBuffer->set(deviceContext);

    for(auto& shader : m_shaders) {
      shader->set(deviceContext);
    }

    m_inputLayout->set(deviceContext);

    prepared = true;
  }

  return prepared;
}

void Technique::release() {
  
  if (m_inputLayout) {
    m_inputLayout->release();
  }

  if (m_constBuffer) {
    m_constBuffer->release();
  }
  
  for (auto& shader : m_shaders) {
    if (shader) {
      shader->release();
    }
  }

  m_inputLayout = nullptr;
  m_constBuffer = nullptr;
  m_shaders.clear();
}

}