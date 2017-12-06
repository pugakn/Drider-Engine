#pragma once

#include <vector>
#include <unordered_set>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_mesh.h"
#include "dr_resource.h"

namespace driderSDK {

class DR_CORE_EXPORT Model : public Resource 
{
 public:

  Model();

  ~Model();

  Model(const Model&) = delete;

  Model& operator=(const Model&) = delete;

  void 
  init(void* pModelData);

  std::vector<Mesh> meshes;
  TString skeletonName;
};

}
