#include "dr_graph.h"
#include <functional>
#include <queue>
#include <dr_vector3d.h>
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

std::vector<RenderMesh>
SceneGraph::query(Camera& camera, QUERY_ORDER::E order, UInt32 props) {

  using ObjectComp = std::function<bool(SharedGameObject, SharedGameObject)>;
  using GameObjectQueue = std::priority_queue<SharedGameObject,
                                              std::vector<SharedGameObject>,
                                              ObjectComp>;

  auto objectComp = [&](SharedGameObject a, SharedGameObject b) -> bool
  {
    auto renderA = a->getComponent<RenderComponent>();
 
    Vector4D posA(renderA->center, 1.f);

    auto WVPA = a->getWorldTransform().getMatrix() * camera.getVP();

    auto renderB = b->getComponent<RenderComponent>();

    Vector4D posB(renderB->center, 1.f);

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

  std::function<void(SharedGameObject)> addObject = [&](SharedGameObject obj)
  {
    if (obj->getComponent<RenderComponent>()) {
      objects.push(obj);
    }

    for (SizeT i = 0; i < obj->getChildrenCount(); ++i) {
      addObject(std::dynamic_pointer_cast<GameObject>(obj->getChild(i)));
    }
  };

  addObject(m_root);

  std::vector<RenderMesh> meshes;
   
  while (!objects.empty()) {

    auto renderCmp = objects.top()->getComponent<RenderComponent>();

    auto& meshesObj = renderCmp->getMeshes();

    meshes.insert(meshes.begin(), meshesObj.begin(), meshesObj.end());

    objects.pop();    
  }

  return meshes;  
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