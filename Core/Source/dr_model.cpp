#include "dr_model.h"
#include <dr_device.h>
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include "dr_model_info.h"
#include "dr_mesh_info.h"

namespace driderSDK {

Model::Model() {}

Model::~Model() {}

void Model::init(void* modelData, void* extraData) {
    
  ModelInfo* model = static_cast<ModelInfo*>(modelData);
  Device* device = static_cast<Device*>(extraData);

  for (auto& meshInfo : model->meshes) {
    Mesh mesh;
    mesh.vertices = std::move(meshInfo.vertices);
    mesh.indices = std::move(meshInfo.indices);

    DrBufferDesc buffDesc;
      
    buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
    buffDesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
    buffDesc.stride = sizeof(Vertex);
    auto buffData = reinterpret_cast<const byte*>(mesh.vertices.data());
    Buffer* buffer = device->createBuffer(buffDesc, buffData);
    mesh.vertexBuffer.reset(dynamic_cast<VertexBuffer*>(buffer));
  
    buffDesc.type = DR_BUFFER_TYPE::kINDEX;
    buffDesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
    buffDesc.stride = 0;
    buffData = reinterpret_cast<const byte*>(mesh.indices.data());
    buffer = device->createBuffer(buffDesc, buffData);
    mesh.indexBuffer.reset(dynamic_cast<IndexBuffer*>(buffer));

    meshes.push_back(std::move(mesh));
  }

  aabb = model->aabb;

  animationsNames = model->animationsNames;
  skeletonName = model->skeletonName;
}

}