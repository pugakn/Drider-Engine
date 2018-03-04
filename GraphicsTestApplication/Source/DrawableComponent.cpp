#include "DrawableComponent.h"
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_gameObject.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_model.h>
#include <dr_render_component.h>
#include <dr_vertex_buffer.h>
#include "Technique.h"

namespace driderSDK {

void 
DrawableComponent::setShaderTechnique(Technique* technique) {
  m_technique = technique;
}

void 
DrawableComponent::onRender() {
  if (m_technique) {
    
    if (m_technique->prepareForDraw()) {

      auto& deviceContext = GraphicsAPI::getDeviceContext();

      deviceContext.setPrimitiveTopology(m_primitive);

      setWorld();

      for (auto& mesh : m_meshes) {
           mesh.vertexBuffer->set(deviceContext);
           mesh.indexBuffer->set(deviceContext);

           deviceContext.draw(mesh.indicesCount, 0, 0);
      }
    }
  } 
}

void 
DrawableComponent::onDestroy() {
  destroyMeshBuffers();
}

void 
DrawableComponent::createMeshBuffers(std::vector<Mesh>& meshes) {
  if (!m_meshes.empty()) {
    destroyMeshBuffers();
  } 

  auto& device = GraphicsAPI::getDevice();
  auto& dc = GraphicsAPI::getDeviceContext();

  for(auto& mesh : meshes)
  {
    MeshBuffers meshBuffer;
    DrBufferDesc buffDesc;
    
    buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
    buffDesc.usage = DR_BUFFER_USAGE::kDynamic;
    buffDesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
    buffDesc.stride = sizeof(Vertex);
    auto buffData = reinterpret_cast<byte*>(mesh.vertices.data());
    Buffer* buffer = device.createBuffer(buffDesc, buffData);
    buffer->updateFromBuffer(dc, buffData);
    meshBuffer.vertexBuffer = dynamic_cast<VertexBuffer*>(buffer);
  
    buffDesc.type = DR_BUFFER_TYPE::kINDEX;
    buffDesc.usage = DR_BUFFER_USAGE::kDefault;
    buffDesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
    buffDesc.stride = 0;
    buffData = reinterpret_cast<byte*>(mesh.indices.data());
    buffer = device.createBuffer(buffDesc, buffData);
    meshBuffer.indexBuffer = dynamic_cast<IndexBuffer*>(buffer);

    meshBuffer.indicesCount = mesh.indices.size();

    m_meshes.push_back(meshBuffer);
  }
}

void
DrawableComponent::updateVertexBuffer(std::vector<Mesh>& meshes) {

  Int32 i = 0;

  auto& dc = GraphicsAPI::getDeviceContext();

  for(auto& mesh : meshes)
  {

    auto newData = reinterpret_cast<byte*>(mesh.vertices.data());

    m_meshes[i].vertexBuffer->updateFromBuffer(dc, newData);

    ++i;
  }

}

void 
DrawableComponent::destroyMeshBuffers() {
  for (auto& mesh : m_meshes) {
    mesh.indexBuffer->release();
    mesh.vertexBuffer->release();
  }

  m_meshes.clear();
}

}