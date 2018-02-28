#include "dr_GBuffer.h"

namespace driderSDK {

GBufferPass::GBufferPass() {
}

GBufferPass::~GBufferPass() {
}

void GBufferPass::init() {
  //Set GBuffer Vertex shader
  m_vertexShader = nullptr;
  //Set GBuffer fragment shader
  m_fragmentShader = nullptr;
}

void GBufferPass::draw(void* data) {
  RenderTarget* OutputColor  = &static_cast<RenderTarget*>(data)[0];
  RenderTarget* OutputNormal = &static_cast<RenderTarget*>(data)[1];
  RenderTarget* OutputDepth  = &static_cast<RenderTarget*>(data)[2];
}

}