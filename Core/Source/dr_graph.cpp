#include "dr_graph.h"

#include <dr_frustrum.h>
#include <dr_id_object.h>
#include <dr_matrix4x4.h>
#include <dr_scoped_timer.h>
#include <dr_vector3d.h>
#include <dr_vector4d.h>

#include "dr_animator_component.h"
#include "dr_camera.h"
#include "dr_gameObject.h"
#include "dr_model.h"
#include "dr_render_component.h"
#include "dr_root_node.h"
#include "dr_octree.h"

#include "dr_script_component.h"
#include "dr_file_system.h"

namespace driderSDK {

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void 
SceneGraph::onStartUp() {
  auto r = std::make_shared<RootNode>();
  r->init();
  m_root = r;

  
  m_octree = std::make_shared<GameObject>(_T("OCTREE_ROOT_X"));
}

void
SceneGraph::onShutDown() {
  m_root->destroy();
  m_octree->destroy();
  m_octree = nullptr;
  m_root = nullptr;
}

void 
SceneGraph::addObject(SharedGameObject gameObject) {
  instance().m_root->addChild(gameObject);
}

void
SceneGraph::instanciate(GameObject& object,
                        const Vector3D& position,
                        const Vector3D& orientation) {
  
  object.getTransform().setPosition(position);
  object.getTransform().setRotation(orientation);

  instance().m_root->addChild(std::make_shared<GameObject>(object));
  object.start();
}

void
SceneGraph::instanciate(SharedGameObject object,
                        SharedGameObject father,
                        const Vector3D& position,
                        const Vector3D& orientation) {

  object->getTransform().setPosition(position);
  object->getTransform().setRotation(orientation);

  if(father.get() != nullptr) {
    father->addChild(object);
  } else {
    instance().m_root->addChild(object);
  }
  object->start();
}

void 
SceneGraph::buildOctree() {
  
  auto& sc = instance();

  sc.m_octree->destroy();
    
  std::vector<std::shared_ptr<GameObject>> staticGameObjects;

  addGameObjectsStatics(*sc.m_root, &staticGameObjects);

  Octree octree(&(*sc.m_octree), &staticGameObjects, 10000);
  octree.buildTree();
}


SceneGraph::SharedGameObject 
SceneGraph::getRoot() {
  return instance().m_root;
}

SceneGraph::SharedGameObject
SceneGraph::getOctree()
{
  return instance().m_octree;
}

void 
SceneGraph::start() {
  
  instance().m_root->start();

}

void
SceneGraph::update() {

  //instance().m_mutex.lock();
  std::lock_guard<std::mutex> fMutex(instance().m_mutex);

  instance().m_root->update();
  //instance().m_mutex.unlock();
}

void 
SceneGraph::draw() {
  //instance().m_mutex.lock();

  instance().m_root->render();
  
  instance().m_octree->render();
  //instance().m_mutex.unlock();
}

//SceneGraph::QueryResult
//SceneGraph::query(const Camera& camera, QUERY_ORDER::E order, UInt32 props) {
//  
//  GameObjectQueue objects(DepthComparer{camera, order});
//
//  Frustrum frustrum(camera.getView(), camera.getProjection());
//
//  //ScopedTimer{},
//  testObject(instance().m_root, frustrum, objects);
//
//  QueryResult queryRes;
//   
//  if (instance().m_octree) {
//    testObjectOct(instance().m_octree, frustrum, objects, true);
//  }
//
//  //ScopedTimer{},
//  filterObjects(objects, queryRes, props);
//
//  return queryRes;  
//}

SceneGraph::QueryBuffer
SceneGraph::query(const RenderQuery& queryInfo) {
  GameObjectQueue objects(DepthComparer{ queryInfo.camera, queryInfo.order });

  Frustrum frustrum(queryInfo.camera.getView(), queryInfo.camera.getProjection());
  QueryBuffer queryRes;
  auto& sg = instance();
  {

    std::lock_guard<std::mutex> fMutex(sg.m_mutex);
    //ScopedTimer{},
    testObject(sg.m_root, frustrum, objects);


    if (instance().m_octree) {
      testObjectOct(sg.m_octree, frustrum, objects, true);
    }

    //ScopedTimer{},
    filterObjects(objects, queryRes, queryInfo.props);
  }
  return std::move(queryRes);
}

SceneGraph::GameObjectList
SceneGraph::queryGameObjects(const Camera& camera, QUERY_ORDER::E order) {

  GameObjectQueue objsPQ(DepthComparer{camera, order});

  testObject(instance().m_root, 
             Frustrum{camera.getView(), camera.getProjection()}, 
             objsPQ);

  GameObjectList objsList;
  
  while(!objsPQ.empty()) {
    objsList.push_back(objsPQ.top());
    objsPQ.pop();
  }

  return objsList;
}

const std::vector<LightComponent*>
SceneGraph::getLightComponents() {
  return lightsList;
}

void
SceneGraph::updateLightsList() {
  lightsList.clear();
  instance().m_root->getRecursiveComponents<LightComponent>(lightsList);
}

void 
SceneGraph::testObjectOct(SharedGameObject object, 
                          const Frustrum& frustrum, 
                          GameObjectQueue& objects, 
                          bool test) {

  bool ins = !test;

  if (test) {
    if (auto renderComp = object->getComponent<RenderComponent>()) {
    
      auto inter = frustrum.intersects(renderComp->getAABB());

      if (inter != FRUSTRUM_INTERSECT::kOutside) {
      
        ins = true;

        if (inter == FRUSTRUM_INTERSECT::kInside) {
          test = false;
        }
      }
    }
  }  
  
  if (ins) {
    if (object->getComponent<RenderComponent>()) {
        object->render();
        objects.push(object);
    }
  }

  auto& children = object->getChildren();

  for (auto& child : children) {
    testObjectOct(child, frustrum, objects, test);
  }  
}

void
SceneGraph::testObject(SharedGameObject object, 
                       const Frustrum& frustrum,
                       GameObjectQueue& objects) {

  auto renderComponent = object->getComponent<RenderComponent>();

  if (renderComponent) {      

    auto inter = frustrum.intersects(renderComponent->getAABB());

    if (inter != FRUSTRUM_INTERSECT::kOutside) {
      /******************************************/
      object->render();
      /******************************************/
      objects.push(object);
    }      
  }

  auto& children = object->getChildren();

  for (auto& child : children) {
    if (child->isEnabled()) {
      testObject(child, frustrum, objects);
    }
  }  
}

//void 
//SceneGraph::filterObjects(GameObjectQueue& objects,
//                          QueryResult& result,
//                          UInt32 props) {
//
//  while (!objects.empty()) {
//
//    auto obj = objects.top();
//
//    const std::vector<Matrix4x4>* bones = nullptr;
//
//    if (auto animComp = obj->getComponent<AnimatorComponent>()) {
//      bones = &animComp->getBonesTransforms();
//    }
//
//    auto& meshes = obj->getComponent<RenderComponent>()->getMeshes();
//
//    UInt32 staticFlag = 0;
//
//    if (obj->isStatic()) {
//      staticFlag |= QUERY_PROPERTY::kStatic;
//    }
//    else {
//      staticFlag |= QUERY_PROPERTY::kDynamic;
//    }
//
//    for (auto& mesh : meshes) {
//      
//      UInt32 meshProps = staticFlag;
//      
//      auto material = mesh.material.lock();
//            
//      if (material) {
//
//        auto t = material->getProperty<FloatProperty>(_T("Transparency"));
//        
//        if (t && t->value < 1.f) {
//          meshProps |= QUERY_PROPERTY::kTransparent;
//        } 
//        else {
//          meshProps |= QUERY_PROPERTY::kOpaque;
//        }
//      }
//      else {
//        meshProps |= QUERY_PROPERTY::kOpaque;
//      }
//
//      if (meshProps == (meshProps & props)) {
//        result.push_back({obj->getWorldTransform().getMatrix(), mesh, bones});
//      }
//    }
//
//    objects.pop();
//  }
//}

void
SceneGraph::filterObjects(GameObjectQueue& objects, QueryBuffer& result, UInt32 props) {
  while (!objects.empty()) {

    auto obj = objects.top();

    auto animComp = obj->getComponent<AnimatorComponent>();
    Int32 bones = -1;
    if (animComp) {

      bones = result.bonesTransforms.size();
      result.bonesTransforms.push_back(animComp->getBonesTransforms());
    }

    auto& meshes = obj->getComponent<RenderComponent>()->getMeshes();

    UInt32 staticFlag = 0;

    if (obj->isStatic()) {
      staticFlag |= QUERY_PROPERTY::kStatic;
    }
    else {
      staticFlag |= QUERY_PROPERTY::kDynamic;
    }

    Int32 world = result.worlds.size();
    result.worlds.push_back(obj->getWorldTransform().getMatrix());
    for (auto& mesh : meshes) {

      UInt32 meshProps = staticFlag;

      auto material = mesh.material.lock();

      if (material) {

        auto t = material->getProperty<FloatProperty>(_T("Transparency"));

        if (t && t->value < 1.f) {
          meshProps |= QUERY_PROPERTY::kTransparent;
        }
        else {
          meshProps |= QUERY_PROPERTY::kOpaque;
        }
      }
      else {
        meshProps |= QUERY_PROPERTY::kOpaque;
      }
      if (meshProps == (meshProps & props)) {
        result.commands.push_back(RenderCommand{ world,
                                                 mesh,
                                                 bones });
      }
    }

    objects.pop();
  }
}

void
SceneGraph::addGameObjectsStatics(GameObject & node,
                                  std::vector<std::shared_ptr<GameObject>>* list)
{
  std::vector<std::shared_ptr<GameObject>> aux;
  for (auto& child : node.getChildren()) {
    if (child->isStatic()) {
      if (child->getComponent<RenderComponent>())
      {
        list->push_back(child);
      }
      addAllChilds(*child, list);
      aux.push_back(child);
    }
    else {
      addGameObjectsStatics(*child, list);
    }
  }

  for (auto child : aux) {
    node.removeChild(child);
  }
}

void
SceneGraph::addAllChilds(GameObject& node,
                         std::vector<std::shared_ptr<GameObject>>* list) {
  for (auto& child : node.getChildren()) {
    if (child->getComponent<RenderComponent>()) {
      list->push_back(child);
    }
    addAllChilds(*child, list);
  }
}

/*Depth Comparer*/
SceneGraph::DepthComparer::DepthComparer(const Camera& _camera, 
                                         QUERY_ORDER::E _order) 
  : m_camera(_camera),
    m_order(_order)
{}

bool
SceneGraph::DepthComparer::operator()(SharedGameObject objA, 
                                      SharedGameObject objB) const {

  auto renderA = objA->getComponent<RenderComponent>();
 
  Vector4D posA(renderA->getAABB().center, 1.f);

  auto WVPA = m_camera.getVP();

  auto renderB = objB->getComponent<RenderComponent>();

  Vector4D posB(renderB->getAABB().center, 1.f);

  auto WVPB = m_camera.getVP();

  auto aW = (posA * WVPA).w;

  auto bW = (posB * WVPB).w;

  if (m_order == QUERY_ORDER::kFrontToBack) {
    return aW > bW;
  }
  else {
    return aW < bW;
  }
}

Int32
SceneGraph::gameObjectsCount() {
  
  Int32 childs = 0;
  
  for(auto child : instance().m_root->getChildren()) {
    childs += child->gameObjectsCount();
  }
  return childs;
}

void
SceneGraph::saveGraph(TString sceneName) {
  FileSystem fileSystem;
  File sceneFile;
  fileSystem.CreateAndOpen(sceneName, sceneFile);

  sceneFile.Write(sizeof(Int32), 0);
}

}