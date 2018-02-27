#include "dr_graph.h"
#include <dr_vector3d.h>
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
SceneGraph::init() {
  m_root = std::make_shared<RootNode>();
}

SceneGraph::SharedGameObject 
SceneGraph::getRoot() const {
  return m_root;
}

std::vector<SceneGraph::SharedGameObject>
SceneGraph::query(Camera& camera, QUERY_ORDER::E order, UInt32 props) {
  
  GameObjectQueue objects(DepthComparer{camera, order});

  Frustrum frustrum(camera.getVP());
  
  testObject(m_root, props, objects);

  std::vector<SharedGameObject> gameobjects;
   
  while (!objects.empty()) {

    gameobjects.push_back(objects.top());
    
    objects.pop();    
  }

  return gameobjects;  
}

void
SceneGraph::update() {
  m_root->update();
}

void 
SceneGraph::draw() {
  m_root->render();
}

SceneGraph::SharedGameObject
SceneGraph::createNode(SharedGameObject parent, SharedModel model) {

  auto node = std::make_shared<GameObject>();

  parent->addChild(node);
    
  node->createComponent<RenderComponent>(model);

  node->createComponent<AABBCollider>(model->aabb);

  return node;
}

void 
SceneGraph::testObject(SharedGameObject object, 
                       UInt32 props, 
                       GameObjectQueue& objects) {

  if (object->getComponent<RenderComponent>() && 
      object->getComponent<AABBCollider>()) {      

      //if (rc->getAABB().intersect(frustrum)) {
      objects.push(object);
      //}      
    }

    auto& children = object->getChildren();

    for (auto& child : children) {
      testObject(child, props, objects);
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