#include "PathRenderer.h"

#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_mesh.h>
#include <dr_vertex_buffer.h>

#include "PathHolder.h"
#include "Technique.h"


namespace driderSDK {

PathRenderer::PathRenderer(PathHolder& _path) 
  : m_path(_path), 
    m_indicesCount(0),
    m_lastPoints(0)
{}

void 
PathRenderer::draw(Technique* technique) {

  auto numPoints = m_path.numPoints();

  if (numPoints < 2) 
    return;

  createBuffers();
  
  if (technique->prepareForDraw()) {
    auto& deviceContext = GraphicsAPI::getDeviceContext();

      deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kLineList);

      m_vertexBuffer->set(deviceContext);
      m_indexBuffer->set(deviceContext);

      deviceContext.draw(m_indicesCount, 0, 0);
  }

}

PathRenderer::~PathRenderer() {
  destroyBuffers();
}

void PathRenderer::createBuffers() {

  destroyBuffers();

  Mesh mesh;

  for (auto& point : m_path.getPoints()) {
    Vertex ver;
    ver.position = Vector4D(point, 1.f);
    mesh.vertices.push_back(ver);
  }

  for (SizeT i = 0; i < m_path.numPoints() - 1; ++i) {
    mesh.indices.push_back(i);
    mesh.indices.push_back(i + 1);
  }
  
  auto& device = GraphicsAPI::getDevice();
  auto& dc = GraphicsAPI::getDeviceContext();

  DrBufferDesc buffDesc;
    
  buffDesc.type = DR_BUFFER_TYPE::kVERTEX;
  buffDesc.usage = DR_BUFFER_USAGE::kDefault;
  buffDesc.sizeInBytes = mesh.vertices.size() * sizeof(Vertex);
  buffDesc.stride = sizeof(Vertex);
  auto buffData = reinterpret_cast<byte*>(mesh.vertices.data());
  Buffer* buffer = device.createBuffer(buffDesc, buffData);
  buffer->updateFromBuffer(dc, buffData);
  m_vertexBuffer = dynamic_cast<VertexBuffer*>(buffer);
  
  buffDesc.type = DR_BUFFER_TYPE::kINDEX;
  buffDesc.usage = DR_BUFFER_USAGE::kDefault;
  buffDesc.sizeInBytes = mesh.indices.size() * sizeof(UInt32);
  buffDesc.stride = 0;
  buffData = reinterpret_cast<byte*>(mesh.indices.data());
  buffer = device.createBuffer(buffDesc, buffData);
  m_indexBuffer = dynamic_cast<IndexBuffer*>(buffer);

  m_indicesCount = mesh.indices.size();
}

void PathRenderer::destroyBuffers() {
  if (m_vertexBuffer) 
    m_vertexBuffer->release();

  if (m_indexBuffer)
    m_indexBuffer->release();
}

}