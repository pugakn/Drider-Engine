#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include "dr_core_prerequisites.h"

/*********************/
#include <dr_plane.h>

namespace driderSDK {

class Frustrum;
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
  using GameObjectList = std::vector<SharedGameObject>;
   
  SceneGraph();

  ~SceneGraph();

  void 
  init();

  SharedGameObject 
  getRoot() const;

  /**
  * Query meshes from the scene graph
  */
  GameObjectList
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
  using ObjectComp = std::function<bool(SharedGameObject, SharedGameObject)>;
  using GameObjectQueue = std::priority_queue<SharedGameObject,
                                              GameObjectList,
                                              ObjectComp>;

  class DepthComparer
  {
  public:
    DepthComparer(Camera& _camera, QUERY_ORDER::E _order);

    bool 
    operator()(SharedGameObject objA, 
               SharedGameObject objB) const;
  private:
    Camera& m_camera;
    QUERY_ORDER::E m_order;
  };
  
  void 
  testObject(SharedGameObject object, 
             Frustrum& frustrum,
             UInt32 props, 
             GameObjectQueue& objects);
private:
  SharedGameObject m_root;
};

}

