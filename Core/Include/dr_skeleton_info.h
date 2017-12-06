#pragma once

#include <unordered_set>
#include <vector>
#include <dr_memory.h>
#include <dr_matrix4x4.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct SkeletonInfo 
{
  struct NodeInfo;

  using NodeInfoPtr = std::unique_ptr<NodeInfo>;

  struct NodeInfo
  {
    NodeInfo* pParent;
    String name;
    Matrix4x4 boneOffset;
    Matrix4x4 transform;
    std::vector<NodeInfoPtr> childrenPtrs;
  };

  std::unordered_set<String> bonesNames;
  NodeInfoPtr pRootNode;
  //String name;
};

}
