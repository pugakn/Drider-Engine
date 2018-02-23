#pragma once

#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include "dr_vertex.h"

namespace driderSDK {

class Material;

struct MaterialRenderInfo
{
  Int32 indexStart;
  Int32 indexSize;
  Int32 vertexStart;
  Int32 vertexSize;
  std::weak_ptr<Material> material;
};

class RenderComponent : public GameComponent
{
public:

  using IndexBuffer = std::vector<UInt32>;
  using VertexBuffer = std::vector<Vertex>;
  using MaterialList = std::vector<MaterialRenderInfo>;

  RenderComponent(GameObject& _gameObject,
                  IndexBuffer&& _indexBuffer, 
                  VertexBuffer&& _vertexBuffer,
                  MaterialList&& _materials);

  const IndexBuffer&
  getIndexBuffer()
  {
    return m_indexBuffer;
  }

  const VertexBuffer&
  getVertexBuffer()
  {
    return m_vertexBuffer;
  }

  const MaterialList&
  getMaterials()
  {
    return m_materials;
  }

private:
  // Inherited via GameComponent
  virtual 
  void onCreate() override;

  virtual 
  void onUpdate() override;

  virtual 
  void onRender() override;

  virtual 
  void onDestroy() override;

  IndexBuffer m_indexBuffer;
  VertexBuffer m_vertexBuffer;
  MaterialList m_materials;
};

}
