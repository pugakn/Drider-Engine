#include "dr_graph.h"
#include <functional>
#include <queue>
#include <dr_vector3d.h>
#include <dr_frustrum.h>
#include "dr_camera.h"
#include "dr_gameObject.h"
#include "dr_model.h"
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

  using ObjectComp = std::function<bool(SharedGameObject, SharedGameObject)>;
  using GameObjectQueue = std::priority_queue<SharedGameObject,
                                              std::vector<SharedGameObject>,
                                              ObjectComp>;

  auto objectComp = [&](SharedGameObject a, SharedGameObject b) -> bool
  {
    auto renderA = a->getComponent<RenderComponent>();
 
    Vector4D posA(renderA->getCenter(), 1.f);

    auto WVPA = a->getWorldTransform().getMatrix() * camera.getVP();

    auto renderB = b->getComponent<RenderComponent>();

    Vector4D posB(renderB->getCenter(), 1.f);

    auto WVPB = b->getWorldTransform().getMatrix() * camera.getVP();

    auto aW = (posA * WVPA).w;

    auto bW = (posB * WVPB).w;

    if (order == QUERY_ORDER::kFrontToBack) {
      return aW > bW;
    }
    else {
      return aW < bW;
    }
  };
  
  GameObjectQueue objects(objectComp);

  Frustrum frustrum(camera.getVP());

  std::function<void(SharedGameObject)> addObject = [&](SharedGameObject obj)
  {
    if (auto rc = obj->getComponent<RenderComponent>()) {
      
      //if (rc->getAABB().intersect(frustrum)) {
        objects.push(obj);
      //}      
    }

    for (SizeT i = 0; i < obj->getChildrenCount(); ++i) {
      addObject(std::dynamic_pointer_cast<GameObject>(obj->getChild(i)));
    }
  };

  addObject(m_root);

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
  m_root->draw();
}

SceneGraph::SharedGameObject
SceneGraph::createNode(SharedGameObject parent, SharedModel model) {

  auto node = std::make_shared<GameObject>();

  parent->addChild(node);
  
  node->createComponent<RenderComponent>(model);

  return node;
}

}