#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"

namespace driderSDK {

/*
data info structure.
{
RenderTarget* OutputColor;
RenderTarget* OutputNormals;
RenderTarget* OutputDepth;
}
*/
class DR_RENDERMAN_EXPORT GBufferPass : RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  GBufferPass();

  /*
  TEST::testName
  
  Description.
  */
  ~GBufferPass();

  /*
  TEST::testName

  Description.
  */
  void
  init();

  /*
  */
  void
  draw(void* data);
};

}