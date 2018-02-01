#pragma once
#include "dr_core_prerequisites.h"
#include <dr_aabb.h>
#include <dr_math_prerequisites.h>
#include <dr_vector3d.h>
#include <vector>
#include <queue>
#include <list>

namespace driderSDK {

struct Face
{
  Vector3D vertex[3];
};

class DR_CORE_EXPORT Octree
{
 public:
  /**
  * Default constructor
  */
  Octree();

  /**
  * Constructor
  * @param region
  *   Region to initialize octree 
  *
  * @param objects
  *   list object in region
  *
  * @param minAreaSize
  *   minimum size of the octree
  *
  */
  Octree(AABB& region, std::queue<Face> objects, float minAreaSize);

  /**
  * Constructor
  * @param region
  *   Region to initialize octree 
  */
  Octree(AABB& region, float minAreaSize);

  /**
  * Default detructor
  */
  ~Octree();

  /**
  * Octree construction
  */
  void
  BuildTree();

  float minSize;

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  Octree* father;

  std::vector<Octree*> childs;
};

}