#include "FrustumDebug.h"
#include <dr_camera.h>
#include <dr_mesh.h>
#include <dr_aabb.h>

namespace driderSDK {

FrustumDebug::FrustumDebug(GameObject& _go) : DrawableComponent(_go){}

void 
FrustumDebug::create() {

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

void FrustumDebug::onCreate() {
  create();
  m_primitive = DR_PRIMITIVE_TOPOLOGY::kLineList;
}

void FrustumDebug::onUpdate() {
  if (m_gameObject.changed()) {
    create();
  }
}

void 
FrustumDebug::cloneIn(GameObject& _go) {
  auto p = _go.createComponent<FrustumDebug>(); 
  p->m_technique = m_technique;
}

}