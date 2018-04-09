#pragma once
#include "dr_octree.h"
#include <dr_plane.h>

namespace driderSDK {

class Octree;

class DR_CORE_EXPORT OctreeNode
{
 public:
  enum FACESTATES {
    IN,
    OUT,
    INTERSECTS
  };
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
  buildTree(OctreeNode& node);

  void
  createChilds(OctreeNode& node);

  /**
  *
  */
  FACESTATES
  checkFaceAgainstAABB(Face& face, AABB &boundingArea);

  void
  decomposeFace(OctreeNode& node, Face &face, std::vector<Plane>& planes);

  void
  createNewFaces(OctreeNode& node, Face &face, std::vector<Plane>& planes);

  bool
  findIntersectVertex(Vertex &origin,
                      Vertex &end,
                      std::vector<Plane> &planes,
                      Vertex &newVertex);

  void nextFace(OctreeNode* node);

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  std::vector<OctreeNode*> childs;

  std::vector<Plane> planes;
 private:
  Octree* m_octree;
  Int32 m_nextChild = -1;
};

}