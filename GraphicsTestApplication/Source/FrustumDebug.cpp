#include "FrustumDebug.h"

#include <dr_camera.h>
#include <dr_id_object.h>
#include <dr_mesh.h>
#include <dr_aabb.h>
#include <dr_camera.h>

namespace driderSDK {

FrustumDebug::FrustumDebug(GameObject& _go, Camera* _camera) 
  : DrawableComponent(_go, _T("FrustrumDebug")),
    m_camera(_camera)
{}

void 
FrustumDebug::create() {
  
  if (!m_camera) {
    return;
  }

  //Create index buffer & vertex buffer 4 lines
  std::vector<Mesh> meshes{1};

  auto& mesh = meshes[0];
  
  auto view = m_camera->getView();
  auto pro = m_camera->getProjection();

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

UInt32
FrustumDebug::getClassID() const {
  return CLASS_NAME_ID(FrustumDebug);
}

void 
FrustumDebug::onCreate() {
  create();
  m_primitive = DR_PRIMITIVE_TOPOLOGY::kLineList;
}

void 
FrustumDebug::onUpdate() {
  if (m_gameObject.changed()) {
    create();
  }
}

GameComponent*
FrustumDebug::cloneIn(GameObject& _go) {
  auto p = _go.createComponent<FrustumDebug>(m_camera); 

  p->m_technique = m_technique;
  p->m_camera = m_camera;
  p->m_primitive = m_primitive;
  p->m_meshes = m_meshes;
  return p;
}

void 
FrustumDebug::setCamera(Camera* _camera) {
  m_camera = _camera;
}

}