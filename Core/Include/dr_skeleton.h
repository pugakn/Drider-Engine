#pragma once

#include <unordered_map>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Node;

class DR_CORE_EXPORT Skeleton
{
 public:
  using BoneRef = std::shared_ptr<Node>;
  using BonesRefsList = std::unordered_map<TString, BoneRef>;

  Skeleton() {}

  ~Skeleton() {}

  void setBones(BonesRefsList&& bones);

 private:
  BonesRefsList m_bonesRefByName;
};

}