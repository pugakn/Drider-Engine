#pragma once

#include <unordered_map>
#include <vector>
#include <dr_matrix4x4.h>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

#include <dr_aabb.h>

namespace driderSDK {

class DR_CORE_EXPORT Skeleton : public Resource
{
 public:

  struct NodeData 
  {
    NodeData* pParent = nullptr;
    TString name;
    Matrix4x4 boneOffset;
    Matrix4x4 transform;
    Matrix4x4 finalTransform;
    std::vector<std::unique_ptr<NodeData>> children;
  };

  Matrix4x4 gloabalInverseTransform;
  std::vector<NodeData*> bones;
  std::vector<AABB> bonesAABBs;
  std::unordered_map<TString, UInt32> bonesMapping;
  std::unique_ptr<NodeData> pRoot;
};

}