#include "dr_mesh.h"
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>

namespace driderSDK {

void 
BufferDeleter::operator()(IndexBuffer* buffer) {
  buffer->release();
}

void BufferDeleter::operator()(VertexBuffer* buffer) {
  buffer->release();
}

Mesh::Mesh() 
{}

Mesh::~Mesh() {}
}