#pragma once

#include <unordered_map>
#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct SkeletonInfo 
{
  struct BoneInfo
  {
    BoneInfo* pParent;
    TString name;
    std::vector<BoneInfo*> children;
  };

  std::unordered_map<TString, BoneInfo> bonesByName;

  BoneInfo* pRootBone;
};

}
