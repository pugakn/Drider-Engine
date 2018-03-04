#pragma once
#include "dr_octree.h"


namespace driderSDK {

class Octree;

class DR_CORE_EXPORT OctreeNode
{
 public:
  /**
  * Default constructor
  */
  OctreeNode();

  OctreeNode(Octree *octree);

  /**
  * Default constructor
  */
  OctreeNode(Octree *octree, AABB& region);

  /**
  * Default detructor
  */
  ~OctreeNode();
  

  void
  buildTree();

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  std::vector<OctreeNode*> childs;

 private:
  Octree* m_octree;
};

}