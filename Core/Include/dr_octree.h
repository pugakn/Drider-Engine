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
  * Constructor
  * @param nodeSceneGraph
  *   node where the octree will be created 
  *
  * @param std::vector<std::shared_ptr<GameObject>>& gameObjects
  *   reference to the models that are going to 
  *   decompose into faces and then form the octree
  *
  * @param minFacesArea
  *   minimum faces in area
  *
  *  decomposes the models into faces and determines the abb that contains them
  */
  Octree(GameObject* nodeSceneGraph,
	       std::vector<std::shared_ptr<GameObject>>* gameObjects,
	       Int32 minFacesArea = 10000);
  
  /**
  * Octree construction
  * It generates the octree according to the models 
  * that were sent to it in the construction
  */
  void
  buildTree();

  /**
  * @return returns the minimum number of faces in an octreeNode
  */
  Int32
  getMinFaces();

 private:

  /**
  * configure the node for the scenegraph
  */
  void
  configNode(GameObject *node, OctreeNode *octreeNode);

  /**
  * check and assign the minimum and maximum vertices for the aabb
  */
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