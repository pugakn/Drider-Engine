#pragma once

#include "dr_node.h"

namespace driderSDK {

class Bone : public Node
{
public:
  using Node::Node;
  
  void update();

};

}

