#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include "dr_module.h"
#include "dr_core_prerequisites.h"

/*********************/
#include <dr_plane.h>

namespace driderSDK {

struct RenderMesh;
class Matrix4x4;
class Camera;
class Frustrum;
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
  kTransparent = 0x80000000,
  kOpaque = kTransparent >> 1,
  kStatic = kOpaque >> 1,
  kDynamic = kStatic >> 1
  /*kTriangles = 0x1,
  kLines = 0x2*/
};
}

class DR_CORE_EXPORT SceneGraph : public Module<SceneGraph>
{
public:
  using SharedModel = std::shared_ptr<Model>;
  using SharedGameObject = std::shared_ptr<GameObject>;
  using GameObjectList = std::vector<SharedGameObject>;
  using QueryResult = std::vector<std::pair<Matrix4x4,RenderMesh>>;   

  SceneGraph();

  ~SceneGraph();
 
  static void
  buildOctree();

  static void
  addObject(SharedGameObject gameObject); 

  static SharedGameObject 
  getRoot();

  static SharedGameObject 
  getOctree();

  /**
  * Query meshes from the scene graph
  */
  static QueryResult
  query(Camera& camera, QUERY_ORDER::E order, UInt32 props);
     
  static void 
  update();
  /****************/
  /*     TEMP     */
  static void 
  draw();
  /****************/
  static SharedGameObject
  createNode(SharedGameObject parent, SharedModel model);

private:
  using ObjectComp = std::function<bool(SharedGameObject, SharedGameObject)>;
  using GameObjectQueue = std::priority_queue<SharedGameObject,
                                              GameObjectList,
                                              ObjectComp>;

  /**
  * Class used for ordering gameObjects by depth
  */
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
  onStartUp();

  static void
  testObjectOct(SharedGameObject object,
                Frustrum& frustrum, 
                GameObjectQueue& objects,
                bool test);

  static void 
  testObject(SharedGameObject object, 
             Frustrum& frustrum,
             GameObjectQueue& objects);

  static void
  filterObjects(GameObjectQueue& objects, 
                QueryResult& result, 
                UInt32 props);

  static void
  addGameObjectsStatics(GameObject& node,
                        std::vector<std::shared_ptr<GameObject>>* list);

  static void
  addAllChilds(GameObject& node,
               std::vector<std::shared_ptr<GameObject>>* list);
private:
  SharedGameObject m_root;
  SharedGameObject m_octree;
  std::mutex m_mutex;
};

}

