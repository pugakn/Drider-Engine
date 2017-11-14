#pragma once

#include <vector>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct MeshInfo;

struct DR_CORE_EXPORT ModelInfo 
{
  std::vector<MeshInfo> meshes;
};

}
