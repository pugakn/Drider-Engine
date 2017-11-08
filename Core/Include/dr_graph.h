#pragma once

#include <memory>
#include <dr_prerequisites.h>

namespace driderSDK {

class Node;

class DR_API_EXPORT SceneGraph
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
