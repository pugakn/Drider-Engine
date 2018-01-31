#pragma once
#include "dr_core_prerequisites.h"
#include <dr_aabb.h>
#include <dr_math_prerequisites.h>
#include <dr_vector3d.h>
#include <vector>
#include <queue>

namespace driderSDK {

struct Face
{
  std::vector<Vector3D> vertex;
};

class DR_CORE_EXPORT Octree
{
 public:
  /**
  *
  */
  Octree();

  /**
  *
  */
  Octree(AABB& region, std::queue<Face> faces, float minAreaSize);

  /**
  *
  */
  Octree(AABB& region, float minAreaSize);

  /**
  *
  */
  ~Octree();

  /**
  *
  */
  Int32
  AABBContainsFace(Face& face);

  /**
  *
  */
  void
  BuildTree();

  float minSize;

  std::queue<Face> faces;

  AABB boundingRegion;

  Octree* father;

  std::vector<Octree*> childs;
};

}