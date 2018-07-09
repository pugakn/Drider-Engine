#pragma once

#include <vector>
#include <dr_aabb.h>
#include "dr_core_prerequisites.h"
#include "dr_mesh_info.h"

namespace driderSDK {

struct DR_CORE_EXPORT ModelInfo 
{
  ModelInfo()
  {
  }

  AABB aabb;
  std::vector<MeshInfo> meshes;
  std::vector<TString> animationsNames;
  TString skeletonName;
};

}
