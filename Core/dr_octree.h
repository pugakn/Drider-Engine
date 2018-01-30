#pragma once
#include "dr_core_prerequisites.h"
#include <dr_aabb.h>
#include <dr_math_prerequisites.h>
#include <dr_vector3d.h>
#include <vector>
#include <queue>

#define MINSIZE 1
namespace driderSDK {
  struct Face
  {
    std::vector<Vector3D> vertex;
  };
  class DR_CORE_EXPORT Octree
  {
   public:
    Octree(AABB &region, std::queue<Face> faces);
    Octree(AABB &region);
    std::queue<Face> faces;
    Int32 AABBContainsFace(Face &face);
    void BuildTree();
    Octree();
    ~Octree();
    AABB boundingRegion;
    Octree* father;
    std::vector<Octree*> childs;

    
  };

}
