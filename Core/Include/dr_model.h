#pragma once

#include <vector>
#include <unordered_set>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_mesh.h"
#include "dr_resource.h"
#include "dr_skeleton.h"
#include "dr_skeleton_info.h"

namespace driderSDK {

class DR_CORE_EXPORT Model : public Resource 
{
 public:
  Model();
  ~Model();

  void 
  init(void* pModelData);
 private:

  void 
  buildNodeTree(const SkeletonInfo::NodeInfo* pNodeSrc, 
                Skeleton::NodeData* pBone);

  void
  buildBonesData(Skeleton::NodeData* pBone, 
                 std::unordered_set<String>& bonesNames);

public:
   std::vector<Mesh> m_meshes;
   Skeleton m_skeleton;
};

}
