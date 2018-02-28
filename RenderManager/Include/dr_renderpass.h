#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_shader.h>
#include <dr_render_target.h>

namespace driderSDK {

struct PassInitData
{
  
};

struct PassDrawData
{

};

/*
*/
class RenderPass {
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
  init(PassInitData* initData) = 0;

  /*
  */
  virtual void
  draw(PassDrawData* drawData) = 0;

 protected:
  Shader* m_vertexShader;
  Shader* m_fragmentShader;
};

}