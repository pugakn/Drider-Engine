#pragma once

#include <vector>
#include <unordered_set>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_mesh.h"
#include "dr_resource.h"
#include "dr_aabb.h"

namespace driderSDK {

class DR_CORE_EXPORT Model : public Resource 
{
 public:

  Model();

  ~Model();

  Model(const Model&) = delete;

  Model& operator=(const Model&) = delete;

  void 
  init(void* modelData, void* extraData);
  
  AABB aabb;
  std::vector<Mesh> meshes;
  std::vector<TString> animationsNames;
  TString skeletonName;
};

}
