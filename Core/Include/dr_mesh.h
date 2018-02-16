#pragma once

#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_vertex.h"
#include "dr_material.h"

namespace driderSDK {

struct DR_CORE_EXPORT Mesh
{
  std::vector<Vertex> vertices;
  std::vector<UInt32> indices;
  std::weak_ptr<Material> material;
  //Material
};

struct DR_CORE_EXPORT SkinnedMesh 
{
  Mesh meshInfo;

};

}
