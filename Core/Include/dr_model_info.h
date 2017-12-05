#pragma once

#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_mesh_info.h"
#include "dr_skeleton_info.h"

namespace driderSDK {

struct DR_CORE_EXPORT ModelInfo 
{
  ModelInfo()
  {
  }

  std::vector<MeshInfo> meshes;
  SkeletonInfo skeleton;
};

}
