#pragma once
#include "dr_core_prerequisites.h"
#include <dr_aabb.h>
#include <dr_math_prerequisites.h>
#include <dr_vector3d.h>
#include <dr_gameObject.h>
#include <dr_vertex.h>
#include <vector>
#include <queue>
#include <dr_memory.h>
#include <dr_material.h>


namespace driderSDK {
class OctreeNode;

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
  * Default detructor
  */
  ~Octree();

  /**
  * Octree construction
  */
  void
  buildTree();

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
	       std::vector<std::shared_ptr<GameObject>>* gameObjects,
	       Int32 minFacesArea = 10000);

  Int32
  getMinFaces();

 private:

  void
  configNode(GameObject *node, OctreeNode *octreeNode);

  void
  compareMinMax(Vector4D &position);

 private:
  Int32 m_minFaces;

  GameObject* m_rootSceneGraph;

  Vector3D m_minVertex;

  Vector3D m_maxVertex;

  OctreeNode* m_rootOctree;

};

}