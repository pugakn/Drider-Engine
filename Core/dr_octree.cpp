#include "dr_octree.h"

namespace driderSDK {

Octree::Octree() {
}

Octree::Octree(AABB& region,
               std::queue<Face> _faces,
               float minAreaSize = 1.0f) : minSize(minSize) {
  boundingRegion.width = region.width;
  boundingRegion.height = region.height;
  boundingRegion.depth = region.depth;
  boundingRegion.center = region.center;

  faces = _faces;
}

Octree::Octree(AABB& region,
               float minAreaSize = 1.0f) : minSize(minSize) {
  boundingRegion.width = region.width;
  boundingRegion.height = region.height;
  boundingRegion.depth = region.depth;
  boundingRegion.center = region.center;
}

Octree::~Octree() {
}

Int32
Octree::AABBContainsFace(Face& face) {
  Int32 numberOfVertexIn = 0;
  if (boundingRegion.intersect(face.vertex[0])) {
    numberOfVertexIn++;
  }
  if (boundingRegion.intersect(face.vertex[0])) {
    numberOfVertexIn++;
  }
  if (boundingRegion.intersect(face.vertex[0])) {
    numberOfVertexIn++;
  }
  if (numberOfVertexIn > 0) {
    numberOfVertexIn = numberOfVertexIn == 3 ? 2 : 1;
  }
  return numberOfVertexIn;
}

void
Octree::BuildTree() {
  if (faces.size() < 2) {
    return;
  }

  Vector3D size = boundingRegion.getMaxPoint() - boundingRegion.getMinPoint();

  if (size.x <= minSize || size.y <= minSize || size.z <= minSize) {
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
  c5.y -= width;
  Vector3D c6 = c2;
  c6.y -= width;
  Vector3D c7 = c3;
  c7.y -= width;
  Vector3D c8 = c4;
  c8.y -= width;

  regionsChilds.push_back(AABB(width, height, depth, c1));
  regionsChilds.push_back(AABB(width, height, depth, c2));
  regionsChilds.push_back(AABB(width, height, depth, c3));
  regionsChilds.push_back(AABB(width, height, depth, c4));
  regionsChilds.push_back(AABB(width, height, depth, c5));
  regionsChilds.push_back(AABB(width, height, depth, c6));
  regionsChilds.push_back(AABB(width, height, depth, c7));
  regionsChilds.push_back(AABB(width, height, depth, c8));

  for (size_t i = 0; i < regionsChilds.size(); ++i) {
    childs.push_back(new Octree(regionsChilds[i]));
    childs.back()->father = this;
  }

  while (!faces.empty()) {
    for (size_t i = 0; i < regionsChilds.size(); ++i) {
      Int32 info = AABBContainsFace(faces.front());

      if (info == 2) {
        childs[i]->faces.push(faces.front());
        break;
      }
      else if (info == 1) {
        childs[i]->faces.push(faces.front());
      }
    }
    faces.pop();
  }
  return;
}

}