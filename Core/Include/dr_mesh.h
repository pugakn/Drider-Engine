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
  operator()(Buffer*);
};

struct DR_CORE_EXPORT Mesh
{
  using BufferPtrDeleter = std::function<void(Buffer*)>;
  //using VertexPtrDeleter = std::function<void(VertexBuffer*)>;
  using IndexSmartPtr = std::unique_ptr<IndexBuffer, BufferPtrDeleter>;
  using VertexSmartPtro = std::unique_ptr<VertexBuffer, BufferPtrDeleter>;

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
