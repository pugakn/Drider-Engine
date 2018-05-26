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

  void
  divideFacesForPlanes(OctreeNode& node, Face &face);

  void
  findIntersectVertexWhithPlane(Vertex &origin,
                                Vertex &end,
                                Plane &plane,
                                Vertex &newVertex);
  /**
  *
  */
  FACESTATES
  checkFaceAgainstAABB(Face& face, AABB &boundingArea);

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  std::vector<OctreeNode*> childs;

  std::vector<Plane> planes;
 private:
  Octree* m_octree;
};

}