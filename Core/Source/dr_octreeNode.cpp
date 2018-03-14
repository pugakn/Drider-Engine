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
  
  std::vector<Plane> planes;
  Vector3D center = node.boundingRegion.center;
  Vector3D pointLeft = node.boundingRegion.center;
  pointLeft.x += 1;
  Vector3D pointBack = node.boundingRegion.center;
  pointBack.z -= 1;
  Vector3D pointUp = node.boundingRegion.center;
  pointUp.y += 1;


  planes.push_back(Plane(node.boundingRegion.center, pointLeft, pointBack));
  planes.push_back(Plane(node.boundingRegion.center, pointUp, pointBack));
  planes.push_back(Plane(node.boundingRegion.center, pointLeft, pointUp));

  Int32 counter = 0;
  while (!node.objectsToReview.empty()) {
    counter++;
    Face temp = node.objectsToReview.front();
    node.objectsToReview.pop();
    if (temp.vertices[0].position == temp.vertices[1].position ||
      temp.vertices[0].position == temp.vertices[2].position ||
      temp.vertices[2].position == temp.vertices[1].position)
    {
      counter++;
    }
    else {
      decomposeFace(node, temp, planes);
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

OctreeNode::FACESTATES
OctreeNode::checkFaceAgainstAABB(Face& face, AABB &boundingArea) {
  Int32 numberOfVertexIn = 0;
  for (size_t i = 0; i < 3; i++) {
    if (boundingArea.intersect(Vector3D(face.vertices[i].position))) {
      numberOfVertexIn++;
    }
  }

  if (numberOfVertexIn == 0) {
    return FACESTATES::OUT;
  }
  if (numberOfVertexIn == 3) {
    return FACESTATES::IN;
  }
  
  return FACESTATES::INTERSECTS;
}

void
OctreeNode::decomposeFace(OctreeNode& node, 
                          Face & face, 
                          std::vector<Plane>& planes) {
  Face temp = face;
  
  temp.vertices.push_back(temp.vertices[0]);
  temp.vertices.push_back(temp.vertices[1]);
  temp.indices.push_back(temp.indices[0]);
  temp.indices.push_back(temp.indices[1]);

  bool flag[2];
  UInt32 counter = 0;
  Face final = face;
  for (auto& child : node.childs) {
    if (counter == 2) {
      if (FACESTATES::IN == checkFaceAgainstAABB(final, child->boundingRegion)) {
        child->objectsToReview.push(final);
      }
    }
    else {
      for (Int32 i = 0; i < 3; i++) {
        flag[0] = false;
        flag[1] = false;

        if (child->boundingRegion.intersect(Vector3D(temp.vertices[i].position))) {
        
          final.vertices[0] = temp.vertices[i];
          final.indices[0] = temp.indices[i];

          Vertex newVertex;
          counter = 0;
          for (UInt32 j = 1; j < 3; j++)
          {
            if (child->boundingRegion.intersect(Vector3D(temp.vertices[i+j].position))) {
              counter++;
              final.vertices[j] = temp.vertices[i + j];
              final.indices[j] = temp.indices[i + j];
            }
            else {
              if (findIntersectVertex(&temp.vertices[i],
                                      &temp.vertices[i+j],
                                      planes,
                                      newVertex,
                                      child->boundingRegion)) {
                counter++;
                flag[j - 1] = true;
                final.vertices[j] = newVertex;
                std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
                final.indices[j] = (*meshes)[face.mesh];
                (*meshes)[face.mesh]++;
              }
              else {
                break;
              }
            }
          }

          if (counter == 2) {
            child->objectsToReview.push(final);

            if (flag[0] && flag[1]) {
              Face newFace = face;
              newFace.vertices[0] = final.vertices[1];
              newFace.indices[0] = final.indices[1];
              newFace.vertices[1] = temp.vertices[i+1];
              newFace.indices[1] = temp.indices[i+1];
              newFace.vertices[2] = temp.vertices[i+2];
              newFace.indices[2] = temp.indices[i+2];
              if (newFace.vertices[0].position == newFace.vertices[1].position ||
                newFace.vertices[0].position == newFace.vertices[2].position ||
                newFace.vertices[2].position == newFace.vertices[1].position)
              {
                node.objectsToReview.push(newFace);
              }
              else
              {

                node.objectsToReview.push(newFace);
              }
              //push
              newFace.vertices[1] = temp.vertices[i + 2];
              newFace.indices[1] = temp.indices[i + 2];
              newFace.vertices[2] = final.vertices[2];
              newFace.indices[2] = final.indices[2];
              if (newFace.vertices[0].position == newFace.vertices[1].position ||
                newFace.vertices[0].position == newFace.vertices[2].position ||
                newFace.vertices[2].position == newFace.vertices[1].position)
              {
                node.objectsToReview.push(newFace);
              }
              else
              {

                node.objectsToReview.push(newFace);
              }
            }
            else if (flag[0]) {
              Face newFace = face;
              newFace.vertices[0] = final.vertices[1];
              newFace.indices[0] = final.indices[1];
              newFace.vertices[1] = temp.vertices[i + 1];
              newFace.indices[1] = temp.indices[i + 1];
              newFace.vertices[2] = temp.vertices[i + 2];
              newFace.indices[2] = temp.indices[i + 2];
              if (newFace.vertices[0].position == newFace.vertices[1].position ||
                newFace.vertices[0].position == newFace.vertices[2].position ||
                newFace.vertices[2].position == newFace.vertices[1].position)
              {
                node.objectsToReview.push(newFace);
              }
              else
              {

                node.objectsToReview.push(newFace);
              }
            }
            else if (flag[1]) 
            {
              Face newFace = face;
              newFace.vertices[0] = final.vertices[1];
              newFace.indices[0] = final.indices[1];
              newFace.vertices[1] = temp.vertices[i + 2];
              newFace.indices[1] = temp.indices[i + 2];
              newFace.vertices[2] = final.vertices[2];
              newFace.indices[2] = final.indices[2];
              if (newFace.vertices[0].position == newFace.vertices[1].position ||
                newFace.vertices[0].position == newFace.vertices[2].position ||
                newFace.vertices[2].position == newFace.vertices[1].position)
              {
                node.objectsToReview.push(newFace);
              }
              else
              {

                node.objectsToReview.push(newFace);
              }
            }
            break;
          }
        }

      }
    }
  }

}

Vector4D lerp(Vector4D& start, Vector4D& end,float proportion) {
  return start + ((end - start) * proportion);
}
bool
OctreeNode::findIntersectVertex(Vertex *origin,
                                Vertex *end,
                                std::vector<Plane>& planes,
                                Vertex &newVertex,
                                AABB &aabb) {

  Vector3D direction(end->position - origin->position);
  direction.normalize();
  Ray ray(Vector3D(origin->position), direction);

  float distance;
  float minDistance;
  bool flag = false;
  for (Int32 i = 0; i < planes.size(); i++) {
    if (ray.intersects(planes[i], &distance)) {
      if (!flag) {
        if (distance > 0) {
          flag = true;
          minDistance = distance;
        }
      }
      else {
        if (distance > 0 && distance < minDistance) {
          minDistance = distance;
        }
      }
    }
  }
  if (flag && aabb.intersect(ray.origin + ray.direction * minDistance)) {
    float proportion = minDistance / (origin->position - end->position).length();
    newVertex.position = lerp(origin->position, end->position, proportion);
    newVertex.normal = lerp(origin->normal, end->normal, proportion);
    newVertex.binormal = lerp(origin->binormal, end->binormal, proportion);
    newVertex.tangent = lerp(origin->tangent, end->tangent, proportion);
    return true;
  }
  return false;
}
}
