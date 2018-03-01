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
  * @param std::vector<std::shared_ptr<GameObject>>& gameObjects
  *   reference to models to convert in octree
  *
  * @param minFacesArea
  *   minimum faces in area
  *
  */
  Octree(GameObject* nodeSceneGraph,
		 AABB& region,
	     std::vector<std::shared_ptr<GameObject>>* gameObjects,
	     Int32 minFacesArea = 2000);

  /**
  * Constructor
  * @param region
  *   Region to initialize octree 
  */
  Octree(AABB& region,
	     Int32 minFacesArea = 2000);

  /**
  * Default detructor
  */
  ~Octree();

  /**
  * Octree construction
  */
  void
  buildTree();

  void
  createNodes();

  void
  configNode(GameObject *node, Octree *octreeNode);

  Int32 minFaces;

  GameObject* root;

  std::queue<Face> objectsToReview;

  std::vector<Face> containedObjects;

  AABB boundingRegion;

  Octree* father;

  std::vector<Octree*> childs;
};

}