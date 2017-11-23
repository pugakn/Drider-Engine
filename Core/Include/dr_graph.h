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
  void setRoot(SharedNode root);
  void update();
private:
  SharedNode m_root;
};

}
