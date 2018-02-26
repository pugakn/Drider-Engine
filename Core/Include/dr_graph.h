#pragma once

#include <memory>
#include <vector>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Camera;
class GameObject;
class Model;
class Node;
class RenderComponent;

namespace QUERY_ORDER {
 enum E
 {
   kBackToFront,
   kFrontToBack
 };
}

namespace QUERY_PROPERTYS
{
enum E : UInt32
{
  kTransparent = 0x80000000,
  kOpaque = kTransparent >> 1,
  kTriangles = 0x1,
  kLines = 0x2
};
}

class DR_CORE_EXPORT SceneGraph
{
public:
  
  using SharedModel = std::shared_ptr<Model>;
  using SharedGameObject = std::shared_ptr<GameObject>;
   
  SceneGraph();

  ~SceneGraph();

  void 
  init();

  SharedGameObject 
  getRoot() const;

  /**
  * Query meshes from the scene graph
  */
  std::vector<SharedGameObject>
  query(Camera& camera, QUERY_ORDER::E order, UInt32 props);
     
  void 
  update();
  /****************/
  /*     TEMP     */
  void 
  draw();
  /****************/
  SharedGameObject
  createNode(SharedGameObject parent, SharedModel model);
private:
  SharedGameObject m_root;
};

}

