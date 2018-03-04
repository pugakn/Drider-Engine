#include "CameraDebbug.h"
#include <dr_camera.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_index_buffer.h>
#include <dr_vertex_buffer.h>
#include "Technique.h"
#include "dr_mesh.h"
#include "dr_aabb.h"

namespace driderSDK {

void 
CameraDebbug::create(std::shared_ptr<Model> model) {

  //Create index buffer & vertex buffer 4 lines
  std::vector<Mesh> meshes{1};

  auto& mesh = meshes[0];

  auto camera = dynamic_cast<Camera*>(&m_gameObject);

  auto view = camera->getView();
  auto pro = camera->getProjection();

  float size = 1;

  mesh.vertices = {
    Vertex{Vector4D{-size, size, size, 1}}, Vertex{Vector4D{size, size, size, 1}},
    Vertex{Vector4D{-size, size, 0, 1}}, Vertex{Vector4D{size, size, 0, 1}},
    Vertex{Vector4D{-size, -size, size, 1}}, Vertex{Vector4D{size, -size, size, 1}},
    Vertex{Vector4D{-size, -size, 0, 1}}, Vertex{Vector4D{size, -size, 0, 1}}
  };

  view.inverse();
  pro.inverse();

  for (auto& vertex : mesh.vertices) {
    vertex.position = vertex.position * pro;
    vertex.position = vertex.position * view;
    vertex.position.x /= vertex.position.w;
    vertex.position.y /= vertex.position.w;
    vertex.position.z /= vertex.position.w;
    vertex.position.w = 1.0f;
  }
  
  /**
  kLeftDownBack,
  kRightDownBack,
  kLeftDownFront,
  kRightDownFront,
  kLeftTopBack,
  kRightTopBack,
  kLeftTopFront,
  kRightTopFront
  */

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  /////////////////////////////////////////////////
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);
  /////////////////////////////////////////////////
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);

  createMeshBuffers(meshes);
}

void 
CameraDebbug::onCreate() {
  create(nullptr);
}

void 
CameraDebbug::onUpdate() {
  if (m_gameObject.changed()) {
    create(nullptr);
  }
}

void 
CameraDebbug::onRender() {
  if (m_technique) {

    m_technique->setWorld(&m_gameObject.getWorldTransform().getMatrix());

    if (m_technique->prepareForDraw()) {
      
      auto& deviceContext = GraphicsAPI::getDeviceContext();

      deviceContext.setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kLineList);
      
      for (auto& meshBuff : m_meshes) {
        meshBuff.vertexBuffer->set(deviceContext);
        meshBuff.indexBuffer->set(deviceContext);

        deviceContext.draw(meshBuff.indicesCount, 0, 0);
      }
    }
  } 
}

}