#include "AABBDebug.h"

#include <iostream>

#include <dr_id_object.h>
#include <dr_mesh.h>
#include <dr_aabb_collider.h>
#include <dr_gameObject.h>
#include <dr_matrix4x4.h>
#include <dr_animator_component.h>

namespace driderSDK {

AABBDebug::AABBDebug(GameObject& _gameObject, bool _updateFromGO) 
  : DrawableComponent(_gameObject, _T("AABBDebug")),
    m_updateFromGO(_updateFromGO)
{
  m_primitive = DR_PRIMITIVE_TOPOLOGY::kLineList;
}

void AABBDebug::create() {

  Mesh& mesh = m_meshesCore[0];
 
  auto points = m_aabbD.getBounds();

  for (Int32 i = 0; i < 8; ++i) {
    mesh.vertices[i] = {Vector4D{points[i], 1.0}};
  }

  if (mesh.indices.empty()) {
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
  }


  if (m_meshes.empty()) {
    createMeshBuffers(m_meshesCore);
  }
  else {
    updateVertexBuffer(m_meshesCore);
  }
}

void AABBDebug::setAABB(const AABB& _aabb) {
  m_aabbD = _aabb;
  create();
}

void AABBDebug::onCreate() {

  m_meshesCore.resize(1);
  
  m_meshesCore[0].vertices.resize(8);


  if (m_updateFromGO) {
      m_aabbD = m_gameObject.getComponent<AABBCollider>()->getTransformedAABB();
  }

  create();
}

void AABBDebug::onUpdate() {

  if (m_gameObject.getComponent<AnimatorComponent>()) {
    
    m_aabbD = m_gameObject.getComponent<AABBCollider>()->getTransformedAABB();
    
    create();
  }
  else {
    if (m_gameObject.changed()) {

      if (m_updateFromGO) {
        m_aabbD = m_gameObject.getComponent<AABBCollider>()->getTransformedAABB();
      }

      create();
    }
  }
  
}

GameComponent*
AABBDebug::cloneIn(GameObject& _go) {

  auto dup = _go.createComponent<AABBDebug>(m_updateFromGO);
  
  dup->m_technique = m_technique;
  dup->m_primitive = m_primitive;
  dup->m_meshes = m_meshes;

  std::cout << "Warnign trying to copy AABBDebug" << std::endl;
  
  dup->m_updateFromGO = m_updateFromGO;
  dup->m_aabbD = m_aabbD;

  return dup;
}

UInt32 AABBDebug::getClassID() {
  return CLASS_NAME_ID(AABBDebug);
}

}