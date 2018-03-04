#include <dr_octreeNode.h>
#include <dr_octree.h>

namespace driderSDK {

bool
AABBContainsObject(Face& face, AABB &boundingArea) {
  Int32 numberOfVertexIn = 0;
  if (boundingArea.intersect(Vector3D(face.vertices[0].position))) {
    numberOfVertexIn++;
  }
  if (boundingArea.intersect(Vector3D(face.vertices[1].position))) {
    numberOfVertexIn++;
  }
  if (boundingArea.intersect(Vector3D(face.vertices[2].position))) {
    numberOfVertexIn++;
  }

  return numberOfVertexIn == 3;
}
  
OctreeNode::OctreeNode() {}

OctreeNode::OctreeNode(Octree *octree) {
  m_octree = octree;
}

OctreeNode::OctreeNode(Octree *octree, 
                       AABB& region) {
  m_octree = octree;
  boundingRegion = region;

}

OctreeNode::~OctreeNode() {}

void
 OctreeNode::buildTree() {
  Vector3D size = boundingRegion.getMaxPoint() - boundingRegion.getMinPoint();
  if (driderSDK::Int32(objectsToReview.size()) < m_octree->getMinFaces()) {
    while (!objectsToReview.empty())
    {
      containedObjects.push_back(objectsToReview.front());
      objectsToReview.pop();
    }
    return;
  }

  std::vector<AABB> regionsChilds;
  float width = size.x * 0.5f;
  float height = size.y * 0.5f;
  float depth = size.z * 0.5f;
  Vector3D c1 = boundingRegion.center + Vector3D(-width * 0.5f,
                                                 height * 0.5f,
                                                 depth * 0.5f);
  Vector3D c2 = c1;
  c2.x += width;
  Vector3D c3 = c1;
  c3.z -= depth;
  Vector3D c4 = c3;
  c4.x += width;
  Vector3D c5 = c1;
  c5.y -= height;
  Vector3D c6 = c2;
  c6.y -= height;
  Vector3D c7 = c3;
  c7.y -= height;
  Vector3D c8 = c4;
  c8.y -= height;

  regionsChilds.push_back(AABB(width, height, depth, c1));//-++
  regionsChilds.push_back(AABB(width, height, depth, c2));//+++
  regionsChilds.push_back(AABB(width, height, depth, c3));//-+-
  regionsChilds.push_back(AABB(width, height, depth, c4));//++-
  regionsChilds.push_back(AABB(width, height, depth, c5));//--+
  regionsChilds.push_back(AABB(width, height, depth, c6));//+-+
  regionsChilds.push_back(AABB(width, height, depth, c7));//---
  regionsChilds.push_back(AABB(width, height, depth, c8));//+--

  for (size_t i = 0; i < regionsChilds.size(); ++i) {
    childs.push_back(new OctreeNode(m_octree, regionsChilds[i]));
  }

  bool flagContainsInChild;
  while (!objectsToReview.empty()) {
    flagContainsInChild = false;
    for (size_t i = 0; i < regionsChilds.size(); ++i) {
      if (AABBContainsObject(objectsToReview.front(), childs[i]->boundingRegion))
      {
        flagContainsInChild = true;
        childs[i]->objectsToReview.push(objectsToReview.front());
        i = regionsChilds.size();
      }
    }
    if (!flagContainsInChild)
    {
      containedObjects.push_back(objectsToReview.front());
    }
    objectsToReview.pop();
  }

  for (size_t i = 0; i < regionsChilds.size(); ++i) {
    childs[i]->buildTree();
  }
}
}
