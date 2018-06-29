#include "dr_render_component.h"
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_id_object.h>
#include "dr_gameObject.h"
#include "dr_model.h"
#include "dr_logger.h"

namespace driderSDK {

RenderComponent::RenderComponent(GameObject& _gameObject, 
                                 SharedModel resource)
  : GameComponent(_gameObject, _T("RenderComponent")),
    m_model(resource),
    m_isModel(true) 
{}

RenderComponent::RenderComponent(GameObject& _gameObject, 
                                 MeshList&& _materials) 
  : GameComponent(_gameObject, _T("RenderComponent")),
    m_meshes(std::move(_materials)),
    m_isModel(false)
{}

void 
RenderComponent::setModel(SharedModel model) {
  m_isModel = true;
  m_model = model;
  onCreate();
}

void 
RenderComponent::setMeshes(MeshList&& _meshes) {
  m_isModel = false;
  m_meshes = std::move(_meshes);
}

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

  auto model = m_model.lock();

  if (!model && m_isModel) {
    //setEnabled(false);
    Logger::addLog(_T("Render Component model resource no logger available"));
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

UInt32 RenderComponent::getClassID()
{
  return CLASS_NAME_ID(RenderComponent);
}

GameComponent*
RenderComponent::cloneIn(GameObject& _go) {

  auto dup = _go.createComponent<RenderComponent>(MeshList{});

  /*Make it model so it wont release ib & vb*/
  dup->m_isModel = true;
  dup->m_model = m_model;
  dup->m_meshes = m_meshes;

  return dup;
}

}