#include "dr_renderpass.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_file.h>
#include <dr_string_utils.h>

namespace driderSDK {

RenderPass::RenderPass()
  : m_vertexShader(nullptr),
    m_fragmentShader(nullptr),
    m_computeShader(nullptr) {
}

RenderPass::~RenderPass() {
}

void
RenderPass::recompileShader(String vsPreText,
                            String psPreText,
                            String csPreText) {
  if (m_vertexShader != nullptr) {
    m_vertexShader->release();
    m_vertexShader.release();
  }
  if (m_fragmentShader != nullptr) {
    m_fragmentShader->release();
    m_fragmentShader.release();
  }
  if (m_computeShader != nullptr) {
    m_computeShader->release();
    m_computeShader.release();
  }

  Device& dc = GraphicsAPI::getDevice();
  
  driderSDK::File file;
  String shaderSrc = "";

  if (!m_vsFilename.empty()) {
    file.Open(m_vsFilename);
    shaderSrc = vsPreText + StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();

    m_vertexShader = dr_gfx_unique(dc.createShaderFromMemory(shaderSrc.data(),
                                                             shaderSrc.size(),
                                                             DR_SHADER_TYPE_FLAG::kVertex));

    shaderSrc.clear();
  }

  if (!m_fsFilename.empty()) {
    file.Open(m_fsFilename);
    shaderSrc = psPreText + StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();

    m_fragmentShader = dr_gfx_unique(dc.createShaderFromMemory(shaderSrc.data(),
                                                               shaderSrc.size(),
                                                               DR_SHADER_TYPE_FLAG::kFragment));

    shaderSrc.clear();

    m_inputLayout = dr_gfx_unique(dc.createInputLayout(m_vertexShader->reflect(),
                                                       *m_vertexShader->m_shaderBytecode));
  }

  if (!m_csFilename.empty()) {
    file.Open(m_csFilename);
    shaderSrc = csPreText + StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();

    m_computeShader = dr_gfx_unique(dc.createShaderFromMemory(shaderSrc.data(),
                                                              shaderSrc.size(),
                                                              DR_SHADER_TYPE_FLAG::kCompute));

    shaderSrc.clear();
  }

}

}