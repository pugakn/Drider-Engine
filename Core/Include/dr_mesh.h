#pragma once

#include <vector>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Vertex;

class DR_CORE_EXPORT Mesh
{
 public:
  std::vector<Vertex> vertices;
  std::vector<Int16> indices; 
};

}
