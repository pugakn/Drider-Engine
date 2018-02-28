#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_shader.h>
#include <dr_render_target.h>

namespace driderSDK {

/*
*/
class DR_RENDERMAN_EXPORT RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  RenderPass();

  /*
  */
  ~RenderPass();

  /*
  */
  virtual void
  init() = 0;

  /*
  */
  virtual void
  draw(void* data) = 0;

 protected:
  Shader* m_vertexShader;
  Shader* m_fragmentShader;
};

}