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

  /**
  * @param refrence to octree
  * Constructor
  */
  OctreeNode(Octree *octree);

  /**
  * @param refrence to octree
  *
  * @param aabb region for node
  * Constructor
  */
  OctreeNode(Octree *octree, AABB& region);

  /**
  * Default destructor
  */
  ~OctreeNode();
  
  /**
  * Octree construction
  * It generates the octree 
  */
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