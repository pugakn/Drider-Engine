#include "dr_graph.h"
#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include <dr_matrix4x4.h>
#include <dr_frustrum.h>
#include "dr_aabb_collider.h"
#include "dr_camera.h"
#include "dr_gameObject.h"
#include "dr_model.h"
#include "dr_render_component.h"
#include "dr_root_node.h"

namespace driderSDK {

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void 
SceneGraph::buildOctree() {
  //m_octree
}

void 
SceneGraph::addObject(SharedGameObject gameObject) {
  instance().m_root->addChild(gameObject);
}

SceneGraph::SharedGameObject 
SceneGraph::getRoot() {
  return instance().m_root;
}

void
SceneGraph::update() {

  //instance().m_mutex.lock();

  instance().m_root->update();

  //instance().m_mutex.unlock();
}

void 
SceneGraph::draw() {
  //instance().m_mutex.lock();

  instance().m_root->render();

  //instance().m_mutex.unlock();
}

SceneGraph::SharedGameObject
SceneGraph::createNode(SharedGameObject parent, SharedModel model) {

  auto node = std::make_shared<GameObject>();

  parent->addChild(node);
    
  node->createComponent<RenderComponent>(model);

  node->createComponent<AABBCollider>(model->aabb);

  return node;
}

void SceneGraph::onStartUp() {
  m_root = std::make_shared<RootNode>();
}

SceneGraph::QueryResult
SceneGraph::query(Camera& camera, QUERY_ORDER::E order, UInt32 props) {
  
  GameObjectQueue objects(DepthComparer{camera, order});

  Frustrum frustrum(camera.getView(), camera.getProjection());

  testObject(instance().m_root, frustrum, objects);

  QueryResult queryRes;
   
  if (instance().m_octree) {
    testObjectOct(instance().m_octree, frustrum, objects, true);
  }

  filterObjects(objects, queryRes, props);

  return queryRes;  
}

void 
SceneGraph::testObjectOct(SharedGameObject object, 
                          Frustrum& frustrum, 
                          GameObjectQueue& objects, 
                          bool test) {

  bool ins = !test;

  if (test) {
    auto aabbCollider = object->getComponent<AABBCollider>();

    DR_ASSERT(aabbCollider);

    auto inter = frustrum.intersects(aabbCollider->getTransformedAABB());

    if (inter != FRUSTRUM_INTERSECT::kOutside) {
      
      ins = true;

      if (inter == FRUSTRUM_INTERSECT::kInside) {
        test = false;
      }
    }
  }  
  
  if (ins) {
    if (object->getComponent<RenderComponent>()) {
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
                       Frustrum& frustrum,
                       GameObjectQueue& objects) {

  auto aabbCollider = object->getComponent<AABBCollider>();

  if (object->getComponent<RenderComponent>() && 
      aabbCollider) {      

    auto inter = frustrum.intersects(aabbCollider->getTransformedAABB());

    if (inter != FRUSTRUM_INTERSECT::kOutside) {
      object->render();
      objects.push(object);
    }      
  }

  auto& children = object->getChildren();

  for (auto& child : children) {
    testObject(child, frustrum, objects);
  }  
}

void 
SceneGraph::filterObjects(GameObjectQueue& objects,
                          QueryResult& result,
                          UInt32 props) {

  while (!objects.empty()) {

    auto obj = objects.top();

    auto& meshes = obj->getComponent<RenderComponent>()->getMeshes();

    for (auto& mesh : meshes) {
      
      UInt32 meshProps = 0;

      if (obj->isStatic()) {
        meshProps |= QUERY_PROPERTYS::kStatic;
      }
      else {
        meshProps |= QUERY_PROPERTYS::kDynamic;
      }

      auto material = mesh.material.lock();
            
      if (material) {

        auto t = material->getProperty<FloatProperty>(_T("Transparency"));
        
        if (t && t->value < 1.f) {
          meshProps |= QUERY_PROPERTYS::kTransparent;
        } 
        else {
          meshProps |= QUERY_PROPERTYS::kOpaque;
        }
      }
      else {
        meshProps |= QUERY_PROPERTYS::kOpaque;
      }

      if (meshProps == (meshProps & props)) {
        result.push_back({obj->getWorldTransform().getMatrix(), mesh});
      }
    }

    objects.pop();
  }

}

SceneGraph::DepthComparer::DepthComparer(Camera& _camera, QUERY_ORDER::E _order) 
  : m_camera(_camera),
    m_order(_order)
{}

bool
SceneGraph::DepthComparer::operator()(SharedGameObject objA, 
                                      SharedGameObject objB) const {

  auto renderA = objA->getComponent<AABBCollider>();
 
  Vector4D posA(renderA->getAABB().center, 1.f);

  auto WVPA = objA->getWorldTransform().getMatrix() * m_camera.getVP();

  auto renderB = objB->getComponent<AABBCollider>();

  Vector4D posB(renderB->getAABB().center, 1.f);

  auto WVPB = objB->getWorldTransform().getMatrix() * m_camera.getVP();

  auto aW = (posA * WVPA).w;

  auto bW = (posB * WVPB).w;

  if (m_order == QUERY_ORDER::kFrontToBack) {
    return aW > bW;
  }
  else {
    return aW < bW;
  }
}

}