#include "dr_octree.h"
#include <dr_model.h>
#include <dr_matrix4x4.h>
#include <dr_aabb_collider.h>
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>

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

Octree::Octree() {
}

Octree::Octree(GameObject* nodeSceneGraph,
			   AABB & region,
               std::vector<std::shared_ptr<GameObject>>* gameObjects,
               Int32 minFacesArea) {

  minFaces = minFacesArea;
  root = nodeSceneGraph;
  boundingRegion = region;
  Int32 counterGameObject = 0;
  for (auto& gameObject : (*gameObjects)) {
    
    auto renderComponent = gameObject->getComponent<RenderComponent>();
    auto model = renderComponent->getModel().lock();
    Matrix4x4 transform = gameObject->getWorldTransform().getMatrix();
    
    Int32 counterMesh = 0;
    for (auto& mesh : model->meshes)
    {
      for (size_t i = 0; i < mesh.indices.size(); i = i + 3)
      {
        Face temp;
        temp.vertices.push_back(mesh.vertices[mesh.indices[i]]);
        temp.vertices.back().position = transform * temp.vertices.back().position;
        temp.indices.push_back(mesh.indices[i]);

        temp.vertices.push_back(mesh.vertices[mesh.indices[i + 1]]);
        temp.vertices.back().position = transform * temp.vertices.back().position;
        temp.indices.push_back(mesh.indices[i + 1]);

        temp.vertices.push_back(mesh.vertices[mesh.indices[i + 2]]);
        temp.vertices.back().position = transform * temp.vertices.back().position;
        temp.indices.push_back(mesh.indices[i + 2]);

        temp.material = mesh.material;
        temp.gameObject = counterGameObject;
        temp.mesh = counterMesh;
        objectsToReview.push(temp);
      }
      counterMesh++;
    }

    counterGameObject++;
  }
}

Octree::Octree(AABB& region,
	           Int32 minFacesArea) {
  boundingRegion = region;
  minFaces = minFacesArea;
}

Octree::~Octree() {
}

std::vector<RenderMesh>
createList(std::vector<Face>* faces) {
  struct infoRenderMesh {
	  UInt32 gameobject;
	  UInt32 mesh;
	  std::vector<Vertex> vertices;
	  std::vector<UInt32> indices;
	  std::vector<UInt32> idsVertices;
	  std::weak_ptr<Material> material;
  };

  std::vector<infoRenderMesh> idsRenderMesh;
  infoRenderMesh* ptrIdsRenderMesh = NULL;

  for (auto &face : (*faces)) {
	  bool flag = true;
	  for(auto &id : idsRenderMesh) {
	    if(id.gameobject == face.gameObject && id.mesh == face.mesh) {
           flag = false;
		   ptrIdsRenderMesh = &id;
		   break;
	    }
	  }
	  if (flag)
	  {
		  infoRenderMesh tempInfo;
		  tempInfo.mesh = face.mesh;
		  tempInfo.gameobject = face.gameObject;
		  tempInfo.material = face.material;
		  idsRenderMesh.push_back(tempInfo);
		  ptrIdsRenderMesh = &idsRenderMesh.back();
	  }
	  Int32 indexVertex;
	  Int32 indexIdVertex;

	  for (indexVertex = 0;
		     indexVertex < face.vertices.size();
		     indexVertex++) {
	    for (indexIdVertex = 0;
		     indexIdVertex < Int32(ptrIdsRenderMesh->idsVertices.size());
		     indexIdVertex++) {
	      if(face.indices[indexVertex] == ptrIdsRenderMesh->idsVertices[indexIdVertex]) {
			    break;
		    }
	    }
	    if (indexVertex == Int32(ptrIdsRenderMesh->idsVertices.size())) {
		    ptrIdsRenderMesh->indices.push_back(ptrIdsRenderMesh->vertices.size());
		    ptrIdsRenderMesh->vertices.push_back(face.vertices[indexVertex]);
		    ptrIdsRenderMesh->idsVertices.push_back(face.indices[indexIdVertex]);
	    }
	    else {
		    ptrIdsRenderMesh->indices.push_back(indexIdVertex);
	    }
	  }
  }

  std::vector<RenderMesh> response;
  Device* device = &GraphicsAPI::getDevice();
  for (auto& render : idsRenderMesh) {
	  RenderMesh tempRenderMesh;
	  DrBufferDesc buffDesc;

	  buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
	  buffDesc.sizeInBytes = render.vertices.size() * sizeof(Vertex);
	  buffDesc.stride = sizeof(Vertex);
	  auto buffData = reinterpret_cast<const byte*>(render.vertices.data());
	  Buffer* buffer = device->createBuffer(buffDesc, buffData);

	  tempRenderMesh.vertexBuffer = dynamic_cast<VertexBuffer*>(buffer);

	  buffDesc.type = DR_BUFFER_TYPE::kINDEX;
	  buffDesc.sizeInBytes = render.indices.size() * sizeof(UInt32);
	  buffDesc.stride = 0;
	  buffData = reinterpret_cast<const byte*>(render.indices.data());
	  buffer = device->createBuffer(buffDesc, buffData);

	  tempRenderMesh.indexBuffer = dynamic_cast<IndexBuffer*>(buffer);

	  tempRenderMesh.indicesCount = render.indices.size();
	  tempRenderMesh.material = render.material;

	  response.push_back(tempRenderMesh);
  }
  return response;
}

void
Octree::buildTree() {

  Vector3D size = boundingRegion.getMaxPoint() - boundingRegion.getMinPoint();

  if (driderSDK::Int32(objectsToReview.size()) < minFaces) {
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
    childs.push_back(new Octree(regionsChilds[i], minFaces));
    childs.back()->father = this;
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

void
Octree::createNodes() {
  configNode(root, this);
}

void Octree::configNode(GameObject *nodeRoot, Octree* octreeNode)
{
  nodeRoot->setStatic(true);
  nodeRoot->createComponent<AABBCollider>(boundingRegion);
  std::vector<RenderMesh> list = createList(&containedObjects);
  nodeRoot->createComponent<RenderComponent>(std::move(list));
  
  for (auto& child : (*octreeNode).childs) {
	  std::shared_ptr<GameObject> node = std::make_shared<GameObject>();
	  nodeRoot->addChild(node);
	  configNode(&(*node), child);
  }
}
}