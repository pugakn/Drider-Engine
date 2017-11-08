#include "..\Include\dr_graph.h"
#include <dr_matrix4x4.h>
#include "dr_node.h"


driderSDK::SceneGraph::SceneGraph() {}

driderSDK::SceneGraph::~SceneGraph() {}

void driderSDK::SceneGraph::setRoot(SharedNode root) {
  m_root = root;
}

void driderSDK::SceneGraph::update() {
  //m_root->update()
}
