#include "dr_octree.h"
#include <dr_model.h>
#include <dr_matrix4x4.h>
#include <dr_aabb_collider.h>
#include <dr_device.h>
#include <dr_render_component.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_octreeNode.h>
#include<map>

namespace driderSDK {

Octree::Octree() {
}

Octree::Octree(GameObject* nodeSceneGraph,
               std::vector<std::shared_ptr<GameObject>>* gameObjects,
               Int32 minFacesArea) {

  m_minFaces = 10;
  m_rootSceneGraph = nodeSceneGraph;
  Int32 counterGameObject = 0;
  m_rootOctree = new OctreeNode(this);
  bool flagMinMax = true;
  m_maxVertex;
  m_minVertex;
  for (auto& gameObject : (*gameObjects)) {

    std::vector<UInt32> verticesInMeshes;

    auto renderComponent = gameObject->getComponent<RenderComponent>();
    auto model = renderComponent->getModel().lock();
    Matrix4x4 transform = gameObject->getWorldTransform().getMatrix();

    Int32 counterMesh = 0;
    for (auto& mesh : model->meshes)
    {
      verticesInMeshes.push_back(mesh.indices.size());
      for (size_t i = 0; i < mesh.indices.size(); i = i + 3)
      {
        Face temp;

        for (size_t y = 0; y < 3; y++)
        {
          temp.vertices.push_back(mesh.vertices[mesh.indices[i + y]]);
          temp.vertices.back().position = temp.vertices.back().position * transform;
          temp.indices.push_back(mesh.indices[i + y]);
          if (flagMinMax)
          {
            flagMinMax = false;
            m_maxVertex = temp.vertices.back().position;
            m_minVertex = temp.vertices.back().position;
          }
          compareMinMax(temp.vertices.back().position);
        }

        temp.material = mesh.material;
        temp.gameObject = counterGameObject;
        temp.mesh = counterMesh;
        m_rootOctree->objectsToReview.push(temp);
      }
      counterMesh++;
    }

    counterGameObject++;

    verticesInGameObjects.push_back(verticesInMeshes);

  }

  Vector3D size = m_maxVertex - m_minVertex;
  m_rootOctree->boundingRegion.center = m_minVertex + size * 0.5f;
  m_rootOctree->boundingRegion.width = size.x;
  m_rootOctree->boundingRegion.depth = size.z;
  m_rootOctree->boundingRegion.height = size.y;
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
    std::map<UInt32, UInt32> indicesExist;
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

      auto search = ptrIdsRenderMesh->indicesExist.find(face.indices[indexVertex]);

      if (search != ptrIdsRenderMesh->indicesExist.end()) {
		    ptrIdsRenderMesh->indices.push_back(search->second);
      }
      else {
        ptrIdsRenderMesh->indicesExist[face.indices[indexVertex]] = ptrIdsRenderMesh->vertices.size();
		    ptrIdsRenderMesh->indices.push_back(ptrIdsRenderMesh->vertices.size());
		    ptrIdsRenderMesh->vertices.push_back(face.vertices[indexVertex]);
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
  m_rootOctree->buildTree(*m_rootOctree);
  configNode(m_rootSceneGraph, m_rootOctree);
}

void Octree::configNode(GameObject *nodeRoot, OctreeNode* octreeNode)
{
  nodeRoot->setStatic(true);
  nodeRoot->createComponent<AABBCollider>(octreeNode->boundingRegion);
  std::vector<RenderMesh> list = createList(&octreeNode->containedObjects);
  nodeRoot->createComponent<RenderComponent>(std::move(list));
  
  for (auto& child : (*octreeNode).childs) {
	  std::shared_ptr<GameObject> node = std::make_shared<GameObject>();
	  nodeRoot->addChild(node);
	  configNode(&(*node), child);
  }
  octreeNode->~OctreeNode();
}

void
Octree::compareMinMax(Vector4D &position)
{
  m_minVertex.x = Math::min(position.x, m_minVertex.x);
  m_minVertex.y = Math::min(position.y, m_minVertex.y);
  m_minVertex.z = Math::min(position.z, m_minVertex.z);

  m_maxVertex.x = Math::max(position.x, m_maxVertex.x);
  m_maxVertex.y = Math::max(position.y, m_maxVertex.y);
  m_maxVertex.z = Math::max(position.z, m_maxVertex.z);
}

Int32
Octree::getMinFaces()
{
  return m_minFaces;
}
}