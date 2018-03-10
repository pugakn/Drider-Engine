#include "dr_root_node.h"
#include <dr_transform.h>

namespace driderSDK {
RootNode::RootNode() : GameObject(_T("ROOT_NODE_X")) {}

void RootNode::init() {
  m_nullparent = std::make_shared<GameObject>(_T("NULL_PARENT"));
  
  setParent(m_nullparent);
}

void 
RootNode::updateImpl() {}
}