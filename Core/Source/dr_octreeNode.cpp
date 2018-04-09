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
  if (driderSDK::Int32(node.containedObjects.size()) > node.m_octree->getMinFaces()) {
      for (auto face : node.containedObjects) {
        node.objectsToReview.push(face);
      }
  }
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
OctreeNode::decomposeFace(OctreeNode &node, 
                          Face &face, 
                          std::vector<Plane>& planes) {
  bool flag = true;
  for (auto &child : node.childs) {
    FACESTATES state = checkFaceAgainstAABB(face, child->boundingRegion);
    if (state == FACESTATES::IN) {
      child->objectsToReview.push(face);
      flag = false;
    }
  }
  if (flag) {
      //createNewFaces(node, face, planes);
  }

}

bool checkVertexExists(std::vector<Vertex> &list, Vertex newVertex) {

  for (size_t i = 0; i < list.size(); i++)
  {
    if (list[i].position.equals(newVertex.position, Math::EPSILON)) {
      return true;
    }
  }
  return false;
}

void pushFace(OctreeNode &node,
              Face &face,
              Vector3D &normal) {
  Vector3D cross =
    Vector3D(face.vertices[1].position - face.vertices[0].position).cross(
      Vector3D(face.vertices[2].position - face.vertices[0].position));
  float proportion = cross.dot(normal);
    Face newface = face;
  if (proportion < 0)
  {
    newface.vertices[1] = face.vertices[2];
    newface.indices[1] = face.indices[2];
    newface.vertices[2] = face.vertices[1];
    newface.indices[2] = face.indices[1];
  }
  if (newface.vertices[0].position.equals(newface.vertices[1].position, .001f))
  {
    newface.mia = true;
  }
  newface.mia = true;
  Plane facePlane(Vector3D(newface.vertices[0].position),
                  Vector3D(newface.vertices[1].position),
                  Vector3D(newface.vertices[2].position));
  node.objectsToReview.push(newface);
}

bool
SameSide(Vector3D &p1, Vector3D &p2, Vector3D &A, Vector3D &B)
{
  Vector3D cp1 = (B - A).cross(p1 - A);
  Vector3D cp2 = (B - A).cross(p2 - A);
  if (cp1.dot(cp2) >= 0) return true;
  return false;
}

bool 
PointInTriangle(Vector3D P, std::vector<Vertex> &list)
{
  Vector3D A(list[0].position);
  Vector3D B(list[1].position);
  Vector3D C(list[2].position);

  if (SameSide(P, A, B, C) && SameSide(P, B, A, C) && SameSide(P, C, A, B))
  {
    Vector3D vc1 = (A - B).cross(A - C);
    if (Math::abs((A - P).dot(vc1)) <= .01f)
      return true;
  }

  return false;
}

void
OctreeNode::createNewFaces(OctreeNode &node,
                           Face &face,
                           std::vector<Plane> &planes) {
  Face tempFace = face;
  tempFace.vertices.push_back(tempFace.vertices[0]);
  tempFace.indices.push_back(tempFace.indices[0]);

  std::vector<Vertex> listVertices;
  std::vector<UInt32> listIndices;

  for (size_t countVert = 0; countVert < face.vertices.size(); countVert++) {
    if (!checkVertexExists(listVertices, face.vertices[countVert])) {
      listVertices.push_back(face.vertices[countVert]);
      listIndices.push_back(face.indices[countVert]);
    }
    /*for (size_t i = 0; i < planes.size(); i++) {
      Vertex temp;
      if (findIntersectVertex(tempFace.vertices[countVert],
                              tempFace.vertices[countVert + 1],
                              planes[i], temp)) {
        if (!checkVertexExists(listVertices, temp)) {
          std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
          listIndices.push_back((*meshes)[face.mesh]);
          (*meshes)[face.mesh]++;
          listVertices.push_back(temp);
        }
      }
    }*/
  }
  Plane facePlane(Vector3D(face.vertices[0].position),
                  Vector3D(face.vertices[1].position),
                  Vector3D(face.vertices[2].position));

  Vector3D rayXOrigin = node.boundingRegion.center;
  rayXOrigin.x -= node.boundingRegion.width;
  Vector3D rayXDir = (node.boundingRegion.center - rayXOrigin).normalize();
  Ray rayX(rayXOrigin, rayXDir);
  float distance;
  if (rayX.intersects(facePlane, &distance)) {
    if (distance > Math::SMALL_NUMBER) {
      //create temp
      Vertex temp = face.vertices[0];
      temp.position = rayX.origin + rayX.direction * distance;
      if (!checkVertexExists(listVertices, temp) && PointInTriangle(Vector3D(temp.position), face.vertices)) {
        std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
        listIndices.push_back((*meshes)[face.mesh]);
        (*meshes)[face.mesh]++;
        listVertices.push_back(temp);
      }
    }
  }

  Vector3D rayYOrigin = node.boundingRegion.center;
  rayYOrigin.y -= node.boundingRegion.height;
  Vector3D rayYDir = (node.boundingRegion.center - rayYOrigin).normalize();
  Ray rayY(rayYOrigin, rayYDir);
  if (rayY.intersects(facePlane, &distance)) {
    if (distance > Math::SMALL_NUMBER) {
      //create temp
      Vertex temp = face.vertices[0];
      temp.position = rayY.origin + rayY.direction * distance;
      if (!checkVertexExists(listVertices, temp) && PointInTriangle(Vector3D(temp.position), face.vertices)) {
        std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
        listIndices.push_back((*meshes)[face.mesh]);
        (*meshes)[face.mesh]++;
        listVertices.push_back(temp);
      }
    }
  }

  Vector3D rayZOrigin = node.boundingRegion.center;
  rayZOrigin.z -= node.boundingRegion.depth;
  Vector3D rayZDir = (node.boundingRegion.center - rayZOrigin).normalize();
  Ray rayZ(rayZOrigin, rayZDir);
  if (rayZ.intersects(facePlane, &distance)) {
    if (distance > Math::SMALL_NUMBER) {
      //create temp
      Vertex temp = face.vertices[0];
      temp.position = rayZ.origin + rayZ.direction * distance;
      if (!checkVertexExists(listVertices, temp) && PointInTriangle(Vector3D(temp.position), face.vertices)) {
        std::vector<UInt32>* meshes = &m_octree->verticesInGameObjects[face.gameObject];
        listIndices.push_back((*meshes)[face.mesh]);
        (*meshes)[face.mesh]++;
        listVertices.push_back(temp);
      }
    }
  }

  Vector3D normalFace =
    Vector3D(face.vertices[1].position - face.vertices[0].position).cross(
    Vector3D(face.vertices[2].position - face.vertices[0].position));

  for (auto &child : node.childs) {
    std::vector<Vertex> childVertices;
    std::vector<UInt32> childIndices;
    for (size_t i = 0; i < listVertices.size(); i++)
    {
      if (child->boundingRegion.intersect(Vector3D(listVertices[i].position))) {
        childVertices.push_back(listVertices[i]);
        childIndices.push_back(listIndices[i]);
      }
    }

    if (childVertices.size() == 3) {
      Face newFace = face;
      if (childVertices[0].position.equals(childVertices[1].position,0.001f))
      {
        newFace = face;
      }
      newFace.vertices[0] = childVertices[0];
      newFace.vertices[1] = childVertices[1];
      newFace.vertices[2] = childVertices[2];

      newFace.indices[0] = childIndices[0];
      newFace.indices[1] = childIndices[1];
      newFace.indices[2] = childIndices[2];
      pushFace(node, newFace, normalFace);
    }
    else if (childVertices.size() == 4) {
      Face newFace = face;
      newFace.vertices[0] = childVertices[0];
      newFace.vertices[1] = childVertices[1];
      newFace.vertices[2] = childVertices[2];

      newFace.indices[0] = childIndices[0];
      newFace.indices[1] = childIndices[1];
      newFace.indices[2] = childIndices[2];
      pushFace(node, newFace, normalFace);

      float distance1 = (newFace.vertices[0].position - newFace.vertices[1].position).lengthSqr();
      float distance2 = (newFace.vertices[0].position - newFace.vertices[2].position).lengthSqr();
      float distance3 = (newFace.vertices[2].position - newFace.vertices[1].position).lengthSqr();

      if (distance1 >= distance2 && distance1 >= distance3) {
        newFace.vertices[0] = childVertices[0];
        newFace.vertices[1] = childVertices[1];
        newFace.vertices[2] = childVertices[3];

        newFace.indices[0] = childIndices[0];
        newFace.indices[1] = childIndices[1];
        newFace.indices[2] = childIndices[3];
      }

      if (distance2 >= distance1 && distance2 >= distance3) {
        newFace.vertices[0] = childVertices[0];
        newFace.vertices[1] = childVertices[2];
        newFace.vertices[2] = childVertices[3];

        newFace.indices[0] = childIndices[0];
        newFace.indices[1] = childIndices[2];
        newFace.indices[2] = childIndices[3];
      }

      if (distance3 >= distance2 && distance3 >= distance1) {
        newFace.vertices[0] = childVertices[1];
        newFace.vertices[1] = childVertices[2];
        newFace.vertices[2] = childVertices[3];

        newFace.indices[0] = childIndices[1];
        newFace.indices[1] = childIndices[2];
        newFace.indices[2] = childIndices[3];
      }
      
      pushFace(node, newFace, normalFace);
    }
    else if (childVertices.size() > 4)
    {
      int lu = 2;
    }
  }
}

Vector4D lerp(Vector4D& start, Vector4D& end,float proportion) {
  return start + ((end - start) * proportion);
}
bool
OctreeNode::findIntersectVertex(Vertex &origin,
                                Vertex &end,
                                std::vector<Plane> &planes,
                                Vertex &newVertex) {

  Vector3D direction(end.position - origin.position);
  float distanceBetweenVertices = direction.lengthSqr();
  direction.normalize();
  Ray ray(Vector3D(origin.position), direction);
 
  float distance;
  float minDistance = -1;
  for (auto &plane : planes) {
    if (ray.intersects(plane, &distance)) {
      if (distance > Math::SMALL_NUMBER  &&
        distance * distance < distanceBetweenVertices) {
        if (minDistance == -1 || distance < minDistance)
        {
          minDistance = distance;
        }
      }
    }
  }
  

  if (minDistance != -1) {
    float proportion = minDistance / (origin.position - end.position).length();
    newVertex.position = lerp(origin.position, end.position, proportion);
    newVertex.normal = lerp(origin.normal, end.normal, proportion);
    newVertex.binormal = lerp(origin.binormal, end.binormal, proportion);
    newVertex.tangent = lerp(origin.tangent, end.tangent, proportion);
  }
  return minDistance != -1;
}

void OctreeNode::nextFace(OctreeNode* node)
{
  if (node->objectsToReview.empty()) {
    return;
    if (node->childs.size() == m_nextChild)
    {
      return;
    }
    if (m_nextChild < 0)
    {
      for (auto child : node->childs) {
        buildTree(*child);
      }
      ++m_nextChild;
    }
    if (node->childs[m_nextChild]->objectsToReview.size() > 0)
    {
      nextFace(node->childs[m_nextChild]);
    }
    else
    {
      ++node->m_nextChild;
    }
    return;
  }
  Face finalFace;
  bool flag[2];
  bool flagInChild;
  Vertex newTempVertex;
 
  Face temp = node->objectsToReview.front();
  finalFace = temp;
  temp.vertices.push_back(temp.vertices[0]);
  temp.vertices.push_back(temp.vertices[1]);
  temp.indices.push_back(temp.indices[0]);
  temp.indices.push_back(temp.indices[1]);
  node->objectsToReview.pop();

  flagInChild = false;
  flag[0] = false;
  flag[1] = false;
  for (size_t i = 0; i < 3; i++) {
    if (flagInChild)
    {
      break;
    }
    for (auto& child : node->childs) {
      if (flagInChild)
      {
        if (child->boundingRegion.intersect(Vector3D(temp.vertices[i].position)) &&
          child->boundingRegion.intersect(Vector3D(temp.vertices[i + 1].position)) &&
          child->boundingRegion.intersect(Vector3D(temp.vertices[i + 2].position))
          )
        {
          child->containedObjects.push_back(finalFace);
        }
      }
      else if (child->boundingRegion.intersect(Vector3D(temp.vertices[i].position))) {
        bool complete = true;
        finalFace.vertices[0] = temp.vertices[i];
        finalFace.indices[0] = temp.indices[i];

        for (size_t j = 1; j < 3; j++)
        {
          if (child->boundingRegion.intersect(Vector3D(temp.vertices[i + j].position))) {
            finalFace.vertices[j] = temp.vertices[i + j];
            finalFace.indices[j] = temp.indices[i + j];
          }
          else
          {
            if (findIntersectVertex(temp.vertices[i], temp.vertices[i + j], node->planes, newTempVertex))
            {
              finalFace.vertices[j] = newTempVertex;
              finalFace.indices[j] = node->m_octree->verticesInGameObjects[temp.gameObject][temp.mesh];
              ++node->m_octree->verticesInGameObjects[temp.gameObject][temp.mesh];
              flag[j - 1] = true;
            }
            else
            {
              complete = false;
            }
          }
        }
        if (complete)
        {
          child->containedObjects.push_back(finalFace);
          flagInChild = true;

          Face newface = finalFace;
          if (flag[0]) {
            newface.vertices[0] = finalFace.vertices[1];
            newface.indices[0] = finalFace.indices[1];

            newface.vertices[1] = temp.vertices[i + 1];
            newface.indices[1] = temp.indices[i + 1];

            newface.vertices[2] = temp.vertices[i + 2];
            newface.indices[2] = temp.indices[i + 2];

            node->objectsToReview.push(newface);
          }

          if (flag[1]) {
            newface.vertices[0] = finalFace.vertices[1];
            newface.indices[0] = finalFace.indices[1];

            newface.vertices[1] = temp.vertices[i + 2];
            newface.indices[1] = temp.indices[i + 2];
            
            newface.vertices[2] = finalFace.vertices[2];
            newface.indices[2] = finalFace.indices[2];
            node->objectsToReview.push(newface);
          }
        }
      }
    }
  }
}

}
