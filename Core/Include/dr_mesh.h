#pragma once

#include <functional>
#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_vertex.h"
#include "dr_material.h"

namespace driderSDK {

class IndexBuffer;
class VertexBuffer;

struct DR_CORE_EXPORT BufferDeleter
{
  void 
  operator()(IndexBuffer* buffer);
  
  void 
  operator()(VertexBuffer* buffer);
};

struct DR_CORE_EXPORT Mesh
{
  using IndexPtrDeleter = std::function<void(IndexBuffer*)>;
  using VertexPtrDeleter = std::function<void(VertexBuffer*)>;
  using IndexSmartPtr = std::unique_ptr<IndexBuffer, IndexPtrDeleter>;
  using VertexSmartPtro = std::unique_ptr<VertexBuffer, VertexPtrDeleter>;

  Mesh();

  Mesh(Mesh&&) = default;

  Mesh(const Mesh&) = delete;
  
  ~Mesh();

  Mesh& operator=(Mesh&&) = default;

  Mesh& operator=(const Mesh&) = delete; 

  std::vector<Vertex> vertices;
  std::vector<UInt32> indices;
  IndexSmartPtr indexBuffer;
  VertexSmartPtro vertexBuffer;
  std::weak_ptr<Material> material;
};

}
