#include <dr_octreeNode.h>
#include <dr_octree.h>
#include <dr_ray.h>

namespace driderSDK {
  
OctreeNode::OctreeNode() {}

OctreeNode::OctreeNode(Octree *octree) {
  m_octree = octree;
}

OctreeNode::OctreeNode(Octree *octree, 
                       AABB& region) {
  m_octree = octree;
  boundingRegion = std::move(region);

}

OctreeNode::~OctreeNode() {}

void
OctreeNode::buildTree(OctreeNode& node) {
  if (driderSDK::Int32(node.objectsToReview.size()) < node.m_octree->getMinFaces()) {
    while (!node.objectsToReview.empty())
    {
      node.containedObjects.push_back(node.objectsToReview.front());
      node.objectsToReview.pop();
    }
    return;
  }

  createChilds(node);
  
  Vector3D center = node.boundingRegion.center;
  Vector3D pointLeft = node.boundingRegion.center;
  pointLeft.x += 1;
  Vector3D pointBack = node.boundingRegion.center;
  pointBack.z -= 1;
  Vector3D pointUp = node.boundingRegion.center;
  pointUp.y += 1;


  node.planes.push_back(Plane(node.boundingRegion.center, pointLeft, pointBack));
  node.planes.push_back(Plane(node.boundingRegion.center, pointUp, pointBack));
  node.planes.push_back(Plane(node.boundingRegion.center, pointLeft, pointUp));

  Int32 counter = 0;
  while (!node.objectsToReview.empty()) {
    Face temp = node.objectsToReview.front();
    node.objectsToReview.pop();

    bool flag = true;
    Int32 counterChilds = 0;
    while (counterChilds < node.childs.size())
    {
      OctreeNode* child = node.childs[counterChilds];

      FACESTATES state = checkFaceAgainstAABB(temp, child->boundingRegion);
      if (state == FACESTATES::IN) {
        flag = false;
        child->objectsToReview.push(temp);
      }
      else if (state == FACESTATES::INTERSECTS) {
        flag = false;
        divideFacesForPlanes(node, temp);
        counterChilds = node.childs.size();
      }
      counterChilds++;
    }
    if (flag) {
      divideFacesForPlanes(node, temp);
    }
  }

  for (size_t i = 0; i < node.childs.size(); ++i) {
    buildTree(*node.childs[i]);
  }
}

void
OctreeNode::createChilds(OctreeNode &node) {
  Vector3D size = node.boundingRegion.getMaxPoint() - node.boundingRegion.getMinPoint();

  std::vector<AABB> regionsChilds;
  float width = size.x * 0.5f;
  float height = size.y * 0.5f;
  float depth = size.z * 0.5f;
  Vector3D c1 = node.boundingRegion.center + Vector3D(-width * 0.5f,
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
    node.childs.push_back(new OctreeNode(m_octree, regionsChilds[i]));
  }
}

void
OctreeNode::divideFacesForPlanes(OctreeNode &node,
  Face &face) {

  Int32 front = 0;
  Int32 behind = 0;
  Int32 sizeVertex = face.vertices.size();
  PLANE_INTERSECT::E pointLocation[3];

  bool flagDivide = false; //solo control quitar despues

  for (auto plane : node.planes)
  {
    if (flagDivide) {
      break;
    }
    front = 0;
    behind = 0;
    for (Int32 i = 0; i < sizeVertex; i++) {
      pointLocation[i] = plane.intersects(Vector3D(face.vertices[i].position));

      if (pointLocation[i] == PLANE_INTERSECT::kFront) {
        ++front;
      }
      else if (pointLocation[i] == PLANE_INTERSECT::kBehind) {
        ++behind;
      }
    }

    if (front && behind) {

      flagDivide = true;

      std::vector<Vertex> frontVertex;
      std::vector<UInt32>  frontIndices;
      std::vector<Vertex> backVertex;
      std::vector<UInt32> backIndices;


      for (Int32 i = 0; i < sizeVertex; i++) {
        if (pointLocation[i] == PLANE_INTERSECT::kIn) {
          frontVertex.push_back(face.vertices[i]);
          frontIndices.push_back(face.indices[i]);
          backVertex.push_back(face.vertices[i]);
          backIndices.push_back(face.indices[i]);
        }
        else {
          if (pointLocation[i] == PLANE_INTERSECT::kFront) {
            frontVertex.push_back(face.vertices[i]);
            frontIndices.push_back(face.indices[i]);
          }
          else {
            backVertex.push_back(face.vertices[i]);
            backIndices.push_back(face.indices[i]);
          }
          Int32 currentVertex = (i + 1) % sizeVertex;
          if (!(pointLocation[currentVertex] == PLANE_INTERSECT::kIn || pointLocation[currentVertex] == pointLocation[i])) {
            Vertex newVertex;
            findIntersectVertexWhithPlane(face.vertices[i], face.vertices[currentVertex], plane, newVertex);
            std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
            Int32 index = (*meshes)[face.mesh];
            (*meshes)[face.mesh]++;

            frontVertex.push_back(newVertex);
            frontIndices.push_back(index);
            backVertex.push_back(newVertex);
            backIndices.push_back(index);
          }
        }
      }
      if (frontVertex.size() == 3) {
        Face newface;
        newface = face;
        newface.vertices = frontVertex;
        newface.indices = frontIndices;
        node.objectsToReview.push(newface);
      }
      else if (frontVertex.size() == 4)
      {
        Face newface;
        newface = face;
        newface.vertices[0] = frontVertex[0];
        newface.vertices[1] = frontVertex[1];
        newface.vertices[2] = frontVertex[2];

        newface.indices[0] = frontIndices[0];
        newface.indices[1] = frontIndices[1];
        newface.indices[2] = frontIndices[2];
        node.objectsToReview.push(newface);

        newface.vertices[0] = frontVertex[0];
        newface.vertices[1] = frontVertex[2];
        newface.vertices[2] = frontVertex[3];

        newface.indices[0] = frontIndices[0];
        newface.indices[1] = frontIndices[2];
        newface.indices[2] = frontIndices[3];
        node.objectsToReview.push(newface);
      }
      if (backVertex.size() == 3) {
        Face newface;
        newface = face;
        newface.vertices = backVertex;
        newface.indices = backIndices;
        node.objectsToReview.push(newface);
      }
      else if (backVertex.size() == 4)
      {
        Face newface;
        newface = face;
        newface.vertices[0] = backVertex[0];
        newface.vertices[1] = backVertex[1];
        newface.vertices[2] = backVertex[2];

        newface.indices[0] = backIndices[0];
        newface.indices[1] = backIndices[1];
        newface.indices[2] = backIndices[2];
        node.objectsToReview.push(newface);

        newface.vertices[0] = backVertex[0];
        newface.vertices[1] = backVertex[2];
        newface.vertices[2] = backVertex[3];

        newface.indices[0] = backIndices[0];
        newface.indices[1] = backIndices[2];
        newface.indices[2] = backIndices[3];
        node.objectsToReview.push(newface);
      }
    }
  }
}

void
OctreeNode::findIntersectVertexWhithPlane(Vertex &origin,
  Vertex &end,
  Plane &plane,
  Vertex &newVertex) {

  Vector3D direction(end.position - origin.position);
  direction.normalize();
  Ray ray(Vector3D(origin.position), direction);

  float distance;
  ray.intersects(plane, &distance);
  newVertex.position = origin.position + (direction * distance);
  Vector3D delta;
  /*delta.x = end.uv.x - origin.uv.x;
  delta.y = end.uv.y - origin.uv.y;
  newVertex.uv.x = origin.uv.x + (delta.x * distance);
  newVertex.uv.y = origin.uv.y + (delta.y * distance);
  newVertex.uv = origin.uv;*/


  delta = end.normal - origin.normal;
  newVertex.normal = origin.normal + (delta * distance);
  newVertex.normal.normalize();

  delta = end.binormal - origin.binormal;
  newVertex.binormal = origin.binormal + (delta * distance);
  newVertex.binormal.normalize();

  delta = end.tangent - origin.tangent;
  newVertex.tangent = origin.tangent + (delta * distance);
  newVertex.tangent.normalize();
}
OctreeNode::FACESTATES
OctreeNode::checkFaceAgainstAABB(Face& face, AABB &boundingArea) {
  Int32 numberOfVertexIn = 0;
  Int32 numberOfVertexIntersects = 0;

  Vector3D aabbMin = boundingArea.getMinPoint();
  Vector3D aabbMax = boundingArea.getMaxPoint();
  for (size_t i = 0; i < 3; i++) {
    Vector4D point = face.vertices[i].position;

    if (point.x >= aabbMin.x && point.x <= aabbMax.x &&
      point.y >= aabbMin.y && point.y <= aabbMax.y &&
      point.z >= aabbMin.z && point.z <= aabbMax.z) {
      if (point.x == aabbMin.x || point.x == aabbMax.x ||
        point.y == aabbMin.y || point.y == aabbMax.y ||
        point.z == aabbMin.z || point.z == aabbMax.z) {
        ++numberOfVertexIntersects;
      }
      else {
        ++numberOfVertexIn;
      }
    }
  }

  if (numberOfVertexIn + numberOfVertexIntersects == 3) {
    return FACESTATES::IN;
  }
  if (numberOfVertexIn != 0) {
    return FACESTATES::INTERSECTS;
  }
  return FACESTATES::OUT;
}

}
