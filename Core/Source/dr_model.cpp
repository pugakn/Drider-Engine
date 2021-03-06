#include "dr_model.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include "dr_mesh_info.h"
#include "dr_model_info.h"

namespace driderSDK {

Model::Model() {}

Model::~Model() {}

void Model::init(void* modelData) {
    
  ModelInfo* model = static_cast<ModelInfo*>(modelData);
  Device* device = &GraphicsAPI::getDevice();

  for (auto& meshInfo : model->meshes) {
    Mesh mesh;

    mesh.material = meshInfo.material;

    mesh.vertices = std::move(meshInfo.vertices);
    mesh.indices = std::move(meshInfo.indices);

    DrBufferDesc buffDesc;
      
    buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
    buffDesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
    buffDesc.stride = sizeof(Vertex);
    auto buffData = reinterpret_cast<const byte*>(mesh.vertices.data());
    Buffer* buffer = device->createBuffer(buffDesc, buffData);
    mesh.vertexBuffer = Mesh::VertexSmartPtro(dynamic_cast<VertexBuffer*>(buffer),
                                              BufferDeleter{});
  
    buffDesc.type = DR_BUFFER_TYPE::kINDEX;
    buffDesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
    buffDesc.stride = 0;
    buffData = reinterpret_cast<const byte*>(mesh.indices.data());
    buffer = device->createBuffer(buffDesc, buffData);
    mesh.indexBuffer = Mesh::IndexSmartPtr(dynamic_cast<IndexBuffer*>(buffer),
                                           BufferDeleter{});

    meshes.push_back(std::move(mesh));
  }

  aabb = model->aabb;

  animationsNames = model->animationsNames;
  skeletonName = model->skeletonName;
}

}