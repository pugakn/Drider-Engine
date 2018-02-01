#include "dr_graph.h"
#include "dr_root_node.h"

namespace driderSDK {

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void 
SceneGraph::init() {
  m_root = std::make_shared<RootNode>();
}

//void 
//SceneGraph::setRoot(SharedNode root) {
//  m_root = root;
//}

SceneGraph::SharedNode SceneGraph::getRoot() const {
  return m_root;
}

void SceneGraph::update() {
  m_root->update();
}

void SceneGraph::draw() {
  m_root->draw();
}

}