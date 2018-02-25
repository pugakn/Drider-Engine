#include "dr_graph.h"
#include "dr_model.h"
#include "dr_root_node.h"
#include "dr_gameObject.h"
#include "dr_render_component.h"

namespace driderSDK {

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void 
SceneGraph::init() {
  m_root = std::make_shared<RootNode>();
}

SceneGraph::SharedNode 
SceneGraph::getRoot() const {
  return m_root;
}

void 
SceneGraph::update() {
  m_root->update();
}

void 
SceneGraph::draw() {
  m_root->draw();
}

SceneGraph::SharedGO
SceneGraph::createNode(SharedNode parent, SharedModel model) {

  auto node = std::make_shared<GameObject>();

  parent->addChild(node);
  
  node->createComponent<RenderComponent>(model);

  return node;
}

}