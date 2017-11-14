#pragma once

#include "dr_core_prerequisites.h"
#include "dr_node.h"

namespace driderSDK {

class DR_CORE_EXPORT Bone : public Node
{
public:
  using Node::Node;
  
  void update();

};

}

