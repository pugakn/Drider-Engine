#pragma once

#include <unordered_map>
#include <vector>
#include <dr_matrix4x4.h>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Node;

class DR_CORE_EXPORT Skeleton
{
 public:

  struct NodeData 
  {
    NodeData* pParent = nullptr;
    String name;
    Matrix4x4 boneOffset;
    Matrix4x4 transform;
    std::vector<std::unique_ptr<NodeData>> children;
  };

  Skeleton() {}

  ~Skeleton() {}

  std::vector<NodeData*> bonesData;
  std::unordered_map<String, UInt32> m_bonesMapping;
  std::unique_ptr<NodeData> pRoot;

private:
};

}