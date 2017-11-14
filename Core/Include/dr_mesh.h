#pragma once

#include <vector>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Vertex;

struct DR_CORE_EXPORT Mesh
{
  std::vector<Vertex> vertices;
  std::vector<Int16> indices;
};

}
