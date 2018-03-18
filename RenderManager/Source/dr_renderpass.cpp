#include "dr_renderpass.h"
#include <dr_graphics_api.h>
#include <dr_file.h>
#include <dr_device.h>

namespace driderSDK {

RenderPass::RenderPass() : m_vertexShader(nullptr), m_fragmentShader(nullptr) {
}

RenderPass::~RenderPass() {
}

void
RenderPass::recompileShader() {
  if (m_vertexShader != nullptr) {
    m_vertexShader->release();
    m_vertexShader.release();
  }
  if (m_fragmentShader != nullptr) {
    m_fragmentShader->release();
    m_fragmentShader.release();
  }

  Device& device = GraphicsAPI::getDevice();
  
  driderSDK::File file;
  String shaderSource;

  file.Open(m_vsFilename);
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_vertexShader = dr_gfx_unique(device.createShaderFromMemory(shaderSource.data(),
                                                               shaderSource.size(),
                                                               DR_SHADER_TYPE_FLAG::kVertex));

  shaderSource.clear();

  file.Open(m_fsFilename);
  shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_fragmentShader = dr_gfx_unique(device.createShaderFromMemory(shaderSource.data(),
                                                                 shaderSource.size(),
                                                                 DR_SHADER_TYPE_FLAG::kFragment));

  shaderSource.clear();

  m_inputLayout = dr_gfx_unique(device.createInputLayout(m_vertexShader->reflect(),
                                                         *m_vertexShader->m_shaderBytecode));
}

}