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
DrawableComponent::DrawableComponent(GameObject& gameObject)
  : GameComponent(gameObject)
{}

void
DrawableComponent::setModel(std::shared_ptr<Model> model) {
  m_model = model;

  if(auto model = m_model.lock()) {
    create(model);
  }
}

void 
DrawableComponent::setShaderTechnique(Technique* technique) {
  m_technique = technique;
  if (m_technique) {
    m_technique->compile();
  }  
}

std::shared_ptr<Model>
DrawableComponent::getModel() const {
  return m_model.lock();
}

void
DrawableComponent::onCreate() {
  m_created = false;  
}

void 
DrawableComponent::onUpdate() {
  if (auto model = getModel()) {
    create(model);
  }
}

void 
DrawableComponent::onRender() {
  if (m_technique) {

    auto& deviceContext = GraphicsAPI::getDeviceContext();

    if (m_technique->prepareForDraw()) {

      deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);
      
      if (auto comp = m_gameObject.getComponent<RenderComponent>()) {
        auto& meshes = comp->getMeshes(); 
        for (auto& mesh : meshes) {
           mesh.vertexBuffer->set(deviceContext);
           mesh.indexBuffer->set(deviceContext);

           deviceContext.draw(mesh.indicesCount, 0, 0);
        }
      }
    }
  } 
}

void 
DrawableComponent::onDestroy() {
  m_technique->destroy();

  destroyMeshBuffers();
  
  m_model = std::weak_ptr<Model>();
}

void 
DrawableComponent::create(std::shared_ptr<Model> model) {
  if (!m_created) {
    m_created = true;
    createMeshBuffers(model->meshes);
  }
}

void 
DrawableComponent::createMeshBuffers(std::vector<Mesh>& meshes) {
  if (!m_meshes.empty()) {
    destroyMeshBuffers();
  } 

  auto& device = GraphicsAPI::getDevice();

  for(auto& mesh : meshes)
  {
    MeshBuffers meshBuffer;
    DrBufferDesc buffDesc;
    
    buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
    buffDesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
    buffDesc.stride = sizeof(Vertex);
    auto buffData = reinterpret_cast<byte*>(mesh.vertices.data());
    Buffer* buffer = device.createBuffer(buffDesc, buffData);
    meshBuffer.vertexBuffer = dynamic_cast<VertexBuffer*>(buffer);
  
    buffDesc.type = DR_BUFFER_TYPE::kINDEX;
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
DrawableComponent::destroyMeshBuffers() {
  for (auto& mesh : m_meshes) {
    mesh.indexBuffer->release();
    mesh.vertexBuffer->release();
  }

  m_meshes.clear();
}

}