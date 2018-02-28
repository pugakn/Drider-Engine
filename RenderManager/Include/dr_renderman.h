#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_GBuffer.h"
#include <dr_render_component.h>
#include <dr_graph.h>

namespace driderSDK {

/*
*/
class DR_RENDERMAN_EXPORT RenderMan {
 public:
  /*
  TEST::testName
  
  Description.
  */
   RenderMan();

  /*
  */
  ~RenderMan();

  /*
  */
  template<typename T, typename... Args>
  void
  init(T t, Args... args);

  /*
  */
  template<typename T, typename... Args>
  void
  draw(T t, Args... args);

  /*
  */
  template<typename T, typename... Args>
  void
  exit(T t, Args... args);

 protected:
  /*
  */
  template <typename T>
  void 
  extractParam(int index, void* out, T t);
  
  /*
  */
  template<typename T, typename... Args>
  void
  extractParam(int index, void* out, T t, Args... args);

  SceneGraph* m_sceneGraph;
  std::vector<std::pair<Matrix4x4, RenderMesh>> queryRequest;


  GBufferPass m_GBufferPass;
};

}