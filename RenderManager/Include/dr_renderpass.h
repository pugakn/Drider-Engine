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
  //template<typename T, typename... Args>
  virtual void init(T t, Args... args) = 0;
  */

  /*
  //template<typename T, typename... Args>
  virtual void draw(T t, Args... args) = 0;
  */

 protected:
  Shader* m_vertexShader;
  Shader* m_fragmentShader;
};

}