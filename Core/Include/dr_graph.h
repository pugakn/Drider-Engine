#pragma once

#include <memory>
#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_render_component.h"

namespace driderSDK {

class Camera;
class GameObject;
class Model;
class Node;

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
  kTransparent = 0xFF000000,
  kOpaque = 0x00FF00000
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
  std::vector<RenderMesh>
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

