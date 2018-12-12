#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "dr_core_prerequisites.h"
#include "dr_module.h"
#include "dr_render_component.h"

/*********************/
#include <dr_aabb.h>
#include <dr_plane.h>

#include <dr_light_component.h>

namespace driderSDK {

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

struct DR_CORE_EXPORT RenderCommand 
{

  RenderCommand(Int32 _worldID,
                const RenderMesh& mesh_,
                Int32 _bones)
    : worldID(_worldID),
      mesh(mesh_),
      bonesID(_bones)
  {}

  ~RenderCommand()
  {}

  Int32 bonesID = -1;
  Int32 worldID;
  RenderMesh mesh;
  AABB aabb;
};

struct DR_CORE_EXPORT RenderQuery
{
  Camera& camera;
  QUERY_ORDER::E order;
  UInt32 props;
};

struct DR_CORE_EXPORT RenderCommandBuffer
{

  using WorldData = Matrix4x4;
  using BonesTransforms = std::vector<Matrix4x4>;

  RenderCommandBuffer() = default;

  RenderCommandBuffer(RenderCommandBuffer&& ) = default;

  RenderCommandBuffer(const RenderCommandBuffer&) = delete;

  RenderCommandBuffer& operator=(const RenderCommandBuffer&) = delete;
  RenderCommandBuffer& operator=(RenderCommandBuffer&&) = default;
  //Store Matrices
  std::vector<WorldData> worlds;
  //Store Bones
  std::vector<BonesTransforms> bonesTransforms;

  std::vector<RenderCommand> commands;
};

class DR_CORE_EXPORT SceneGraph : public Module<SceneGraph>
{
 public:
  using SharedModel = std::shared_ptr<Model>;
  using SharedGameObject = std::shared_ptr<GameObject>;
  using GameObjectList = std::vector<SharedGameObject>;
  using QueryResult = std::vector<RenderCommand>;
  using QueryBuffer = RenderCommandBuffer;
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
    obj->start();
    return obj;
  }

  /**
  * Creates a Transform into the scene game with a specific position and
  * rotation.
  */
  void static
  instanciate(GameObject& object,
              const Vector3D& position,
              const Vector3D& orientation);

  void static
  instanciate(SharedGameObject object,
              SharedGameObject father,
              const Vector3D& position,
              const Vector3D& orientation);

  static SharedGameObject 
  getRoot();

  static SharedGameObject 
  getOctree();

  /**
  * Queries meshes from the scene graph
  */
  static QueryResult
  query(const Camera& camera, QUERY_ORDER::E order, UInt32 props);

  static QueryBuffer
  query(const RenderQuery& queryInfo);

  /**
  * Queries the gameObjects that are inside a camera frustrum.
  */
  static GameObjectList
  queryGameObjects(const Camera& camera, QUERY_ORDER::E order);

  const std::vector<LightComponent*>
  getLightComponents();

  void
  updateLightsList();

  //Called when all initial objects
  //and components are created
  static void 
  start();

  static void 
  update();

  static void
  fillBuffer();

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

  /*static void
  filterObjects(GameObjectQueue& objects, 
                QueryResult& result, 
                UInt32 props);*/

  static void
  filterObjects(GameObjectQueue& objects, 
                QueryBuffer& result, 
                UInt32 props);

  static void
  addGameObjectsStatics(GameObject& node,
                        std::vector<std::shared_ptr<GameObject>>* list);

  static void
  addAllChilds(GameObject& node,
               std::vector<std::shared_ptr<GameObject>>* list);

  static Int32
  gameObjectsCount();

  static void
  saveGraph(TString sceneName);

 private:
  SharedGameObject m_root;
  SharedGameObject m_octree;
  std::mutex m_mutex;
  RenderCommandBuffer m_buffers[2];
  RenderCommandBuffer* m_producer = nullptr;
  RenderCommandBuffer* m_consumer = nullptr;
  std::vector<LightComponent*> lightsList;
};

}