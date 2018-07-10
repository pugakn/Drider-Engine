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

namespace QUERY_PROPERTY
{
enum E : UInt32
{
  kTransparent = 1U << 31,
  kOpaque = kTransparent >> 1,
  kStatic = kOpaque >> 1,
  kDynamic = kStatic >> 1,
  kAny = kTransparent | kOpaque | kStatic | kDynamic
  /*kTriangles = 0x1,
  kLines = 0x2*/
};
}

struct DR_CORE_EXPORT QueryObjectInfo 
{
  const Matrix4x4& world;
  const RenderMesh& mesh;
  const std::vector<Matrix4x4>* bones;
};

class DR_CORE_EXPORT SceneGraph : public Module<SceneGraph>
{
 public:
  using SharedModel = std::shared_ptr<Model>;
  using SharedGameObject = std::shared_ptr<GameObject>;
  using GameObjectList = std::vector<SharedGameObject>;
  using QueryResult = std::vector<QueryObjectInfo>;
  using ObjectComp = std::function<bool(SharedGameObject, SharedGameObject)>;
  using GameObjectQueue = std::priority_queue<SharedGameObject,
                                              GameObjectList,
                                              ObjectComp>;

  SceneGraph();

  ~SceneGraph();
 
  static void
  buildOctree();

  /**
  * Adds an object to the graph (root child).
  */
  static void
  addObject(SharedGameObject gameObject); 

  /**
  * Creates a node of Type T and adds it to the graph (root child).
  * 
  * @param name
  *   Name of the node. 
  */
  template<class T = GameObject>
  static std::shared_ptr<T>
  createObject(const TString& name)
  {
    auto obj = std::make_shared<T>(name);

    addObject(obj);

    return obj;
  }

  static SharedGameObject 
  getRoot();

  static SharedGameObject 
  getOctree();

  /**
  * Queries meshes from the scene graph
  */
  static QueryResult
  query(const Camera& camera, QUERY_ORDER::E order, UInt32 props);

  /**
  * Queries the gameObjects that are inside a camera frustrum.
  */
  static GameObjectList
  queryGameObjects(const Camera& camera, QUERY_ORDER::E order);

  //Called when all initial objects
  //and components are created
  static void 
  start();

  static void 
  update();
  /****************/
  /*     TEMP     */
  static void 
  draw();
  /****************/

 private:

  /**
  * Class used for ordering gameObjects by depth
  */
  class DepthComparer
  {
  public:
    DepthComparer(const Camera& _camera, QUERY_ORDER::E _order);

    bool 
    operator()(SharedGameObject objA, 
               SharedGameObject objB) const;
  private:
    const Camera& m_camera;
    QUERY_ORDER::E m_order;
  };
  
  void
  onStartUp();

  void
  onShutDown();

  static void
  testObjectOct(SharedGameObject object,
                const Frustrum& frustrum, 
                GameObjectQueue& objects,
                bool test);

  static void 
  testObject(SharedGameObject object, 
             const Frustrum& frustrum,
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

