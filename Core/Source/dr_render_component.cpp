#include "dr_render_component.h"
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include "dr_model.h"

namespace driderSDK {

RenderComponent::RenderComponent(GameObject& _gameObject, 
                                 SharedModel resource) 
  : GameComponent(_gameObject),
    m_model(resource),
    m_isModel(true) 
{}

RenderComponent::RenderComponent(GameObject& _gameObject, 
                                 MeshList&& _materials) 
  : GameComponent(_gameObject),
    m_meshes(_materials),
    m_isModel(false)
{}

void
RenderComponent::onCreate() {
  
  if (m_isModel) {
    if (auto model = m_model.lock()) {

      for (auto& mesh : model->meshes) {

        RenderMesh renderMesh;

        renderMesh.material = mesh.material;
        renderMesh.indicesCount = mesh.indices.size();
        renderMesh.indexBuffer = mesh.indexBuffer.get();
        renderMesh.vertexBuffer = mesh.vertexBuffer.get();

        m_meshes.push_back(renderMesh);
      }
    }
  }
}

void 
RenderComponent::onUpdate() {
  if (m_model.expired()) {
    setEnabled(false);
  }
}

void 
RenderComponent::onRender() {}

void 
RenderComponent::onDestroy() {
  
  if (!m_isModel) {
    for (auto& renderMesh : m_meshes) {
      renderMesh.indexBuffer->release();
      renderMesh.vertexBuffer->release();
    }
  }

  m_meshes.clear();
}

}