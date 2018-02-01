#pragma once

#include <memory>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Node;

class DR_CORE_EXPORT SceneGraph
{
public:
  using SharedNode = std::shared_ptr<Node>;
  SceneGraph();
  ~SceneGraph();
  void init();
  //void setRoot(SharedNode root);
  SharedNode getRoot() const;
  void update();
  /****************/
  /*     TEMP     */
  void draw();
  /****************/
private:
  SharedNode m_root;
};

}

