#pragma once

#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_vertex.h"

namespace driderSDK {

class Material;

struct DR_CORE_EXPORT MeshInfo {
  std::vector<Vertex> vertices;
  std::vector<UInt32> indices;
  std::shared_ptr<Material> material;
};

}

