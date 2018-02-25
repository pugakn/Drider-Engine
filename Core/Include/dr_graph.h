#pragma once

#include <memory>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class GameObject;
class Model;
class Node;

class DR_CORE_EXPORT SceneGraph
{
public:
  using SharedModel = std::shared_ptr<Model>;
  using SharedNode = std::shared_ptr<Node>;
  using SharedGameObject = std::shared_ptr<GameObject>;
  
  SceneGraph();

  ~SceneGraph();

  void 
  init();

  SharedNode 
  getRoot() const;

  void 
  update();
  /****************/
  /*     TEMP     */
  void 
  draw();
  /****************/
  SharedGameObject
  createNode(SharedNode parent, SharedModel model);
private:
  SharedNode m_root;
};

}

