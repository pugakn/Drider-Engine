#include "dr_render_component.h"
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_id_object.h>

#include "dr_animator_component.h"
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
                                 MeshList&& _materials,
                                 const AABB& _aabb)
  : GameComponent(_gameObject, _T("RenderComponent")),
    m_meshes(std::move(_materials)),
    m_aabb(_aabb),
    m_isModel(false)
{}

void 
RenderComponent::setModel(SharedModel model) {
  if (model) {
    m_isModel = true;
    m_model = model;
    onCreate();
  }
}

void 
RenderComponent::setMeshes(MeshList&& _meshes) {
  m_isModel = false;
  m_meshes = std::move(_meshes);
}

const AABB& 
RenderComponent::getAABB()
{
  return m_transformedAABB;
}

void
RenderComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Render << "\n";
  file.m_file << StringUtils::toString(getName()) << "\n";

  file.m_file << m_isModel << "\n";

  if(m_isModel) { 
    file.m_file << m_model.lock()->m_modelName << "\n";
    /*file.m_file << m_model.lock()->aabb.width << "\n";
    file.m_file << m_model.lock()->aabb.height << "\n";
    file.m_file << m_model.lock()->aabb.depth << "\n";
    file.m_file << m_model.lock()->aabb.center.x << "\n";
    file.m_file << m_model.lock()->aabb.center.y << "\n";
    file.m_file << m_model.lock()->aabb.center.z << "\n";*/
  }
  /*
  if(isModel) {
		aabb: struct "aabb"; (m_model->aabb)
		modelName: string; (resourceName)
	}

	if(!isModel) {
		"Guardar todas las meshes"
		UInt32 indicesCount;
  		IndexBuffer* indexBuffer;
  		VertexBuffer* vertexBuffer;
  		std::weak_ptr<Material> material;
	}
  */
}

void
RenderComponent::deserialize(TString &data) {

}

void
RenderComponent::onCreate() {
  
  if (m_isModel) {
    
    m_aabb = AABB(0, 0, 0, {});

    if (auto model = m_model.lock()) {
      m_aabb = model->aabb;
      m_meshes.clear();
      for (auto& mesh : model->meshes) {

        RenderMesh renderMesh;

        renderMesh.material = mesh.material;
        renderMesh.indicesCount = mesh.indices.size();
        renderMesh.indexBuffer = mesh.indexBuffer.get();
        renderMesh.vertexBuffer = mesh.vertexBuffer.get();
        
        m_meshes.push_back(renderMesh);
      }
    }

    m_transformedAABB = m_aabb;
    m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
  }
}

void 
RenderComponent::onUpdate() {

  auto model = m_model.lock();

  if (model) {
    //Calculate AABB using animation
    auto animator = m_gameObject.getComponent<AnimatorComponent>();

    if (animator) {
      if (auto skeleton = animator->getSkeleton()) {
        auto& transf = animator->getBonesTransforms();

        auto aabbs = skeleton->bonesAABBs;

        Vector3D min{ Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT };
        Vector3D max{ Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT };

        Int32 index = 0;

        for (auto& aabb : aabbs) {

          aabb.recalculate(transf[index]);

          Vector3D lmin = aabb.getMinPoint();
          Vector3D lmax = aabb.getMaxPoint();

          for (Int32 i = 0; i < 3; ++i) {
            min[i] = Math::min(lmin[i], min[i]);
            max[i] = Math::max(lmax[i], max[i]);
          }

          ++index;
        }

        Vector3D diff = max - min;

        m_transformedAABB = AABB{ diff.x, diff.y, diff.z, (max + min) * 0.5f };
        m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());

        return;
      }
    }
    
    if (m_gameObject.changed()) {
      m_transformedAABB = m_aabb;
      m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
    }
  }

  //if (!model && m_isModel) {
  //  //setEnabled(false);
  //  Logger::addLog(_T("Render Component model resource no logger available"));
  //}
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

  auto dup = _go.createComponent<RenderComponent>(MeshList{}, m_aabb);

  /*Make it model so it wont release ib & vb*/
  dup->m_isModel = true;
  dup->m_model = m_model;
  dup->m_meshes = m_meshes;
  dup->m_aabb = m_aabb;
  dup->m_transformedAABB = m_transformedAABB;

  return dup;
}

}