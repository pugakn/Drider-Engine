#pragma once

#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_mesh_info.h"

namespace driderSDK {

struct DR_CORE_EXPORT ModelInfo 
{
  ModelInfo()
  {
  }

  std::vector<MeshInfo> meshes;
  TString skeletonName;
};

}
