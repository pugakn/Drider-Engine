#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>

namespace driderSDK {

struct SSAOInitData : PassInitData {};

struct SSAODrawData : PassDrawData {
  GFXShared<RenderTarget> InRt;
  GFXShared<RenderTarget> OutRt;
  GFXShared<DepthStencil> dsOptions;
};

class SSAOPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  SSAOPass();

  /*
  TEST::testName
  
  Description.
  */
  ~SSAOPass();

  /*
  TEST::testName

  Description.
  */
  void
  init(PassInitData* initData);

  /*
  */
  void
  draw(PassDrawData* drawData);

 private:
};

}