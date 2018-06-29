#pragma once

#include <vector>
#include <dr_memory.h>
#include <dr_vector3d.h>
#include <dr_aabb.h>
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

namespace driderSDK {

class Model;
class Material;
class IndexBuffer;
class VertexBuffer;

struct DR_CORE_EXPORT RenderMesh
{
  RenderMesh() : indexBuffer(nullptr), vertexBuffer(nullptr) {}

  UInt32 indicesCount;
  IndexBuffer* indexBuffer;
  VertexBuffer* vertexBuffer;
  std::weak_ptr<Material> material;
};

class DR_CORE_EXPORT RenderComponent : public GameComponent
{
 public:
  using MeshList = std::vector<RenderMesh>;
  using SharedModel = std::shared_ptr<Model>;
  using WeakModelRef = std::weak_ptr<Model>;

  RenderComponent(GameObject& _gameObject, 
                  SharedModel model);

  RenderComponent(GameObject& _gameObject, 
                  MeshList&& _meshes);
  
  void
  setModel(SharedModel model);

  void
  setMeshes(MeshList&& _meshes);

  /*const */MeshList&
  getMeshes()
  {
    return m_meshes;
  }

  const WeakModelRef&
  getModel()
  {
    return m_model;
  }
 private:
  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual UInt32
  getClassID() override;

  virtual GameComponent*
  cloneIn(GameObject& _go);

  bool m_isModel;
  WeakModelRef m_model;
  MeshList m_meshes;
};

}
