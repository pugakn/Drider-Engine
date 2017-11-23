#pragma once

#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_vertex.h"

namespace driderSDK {

struct DR_CORE_EXPORT Mesh
{
  std::vector<Vertex> vertices;
  std::vector<Int16> indices;
};

}
