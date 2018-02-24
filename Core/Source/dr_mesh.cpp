#include "dr_mesh.h"
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>

namespace driderSDK {

void BufferDeleter::operator()(Buffer* buffer) {
   buffer->release();
}

Mesh::Mesh() 
  : indexBuffer(nullptr, BufferDeleter{}),
    vertexBuffer(nullptr, BufferDeleter{})
{}

Mesh::~Mesh() {}
}