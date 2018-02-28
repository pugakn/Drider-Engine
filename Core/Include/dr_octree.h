#pragma once
#include "dr_core_prerequisites.h"
#include <dr_aabb.h>
#include <dr_math_prerequisites.h>
#include <dr_vector3d.h>
#include <dr_gameObject.h>
#include <dr_render_component.h>
#include <dr_vertex.h>
#include <vector>
#include <queue>
#include <list>
#include <dr_memory.h>

namespace driderSDK {

struct Face
{
  std::vector<Vertex> vertices;
  std::vector<UInt32> indices;
  std::weak_ptr<Material> material;
  UInt32 gameObject;
  UInt32 mesh;
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
  * @param minFacesArea
  *   minimum faces in area
  *
  */
  Octree(AABB& region, std::queue<Face> objects, driderSDK::Int32 minFacesArea);

  Octree(AABB& region, std::vector<std::shared_ptr<GameObject>>* gameObjects, driderSDK::Int32 minFacesArea);

  /**
  * Constructor
  * @param region
  *   Region to initialize octree 
  */
  Octree(AABB& region, driderSDK::Int32 minFacesArea);

  /**
  * Default detructor
  */
  ~Octree();

  /**
  * Octree construction
  */
  void
  BuildTree();

  driderSDK::Int32 minFaces;

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  Octree* father;

  std::vector<Octree*> childs;
};

}