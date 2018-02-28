#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>

namespace driderSDK {

/*
data infoStructure.
{
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
  template<typename T, typename... Args>
  void init(T t, Args... args);

  /*
  */
  template<typename T, typename... Args>
  void draw(T t, Args... args);

 private:
  struct CBuffer {
    Matrix4x4 WVP;
    Matrix4x4 World;
  };
  
  ConstantBuffer* m_constantBuffer;
  InputLayout* m_inputLayout;
};

}