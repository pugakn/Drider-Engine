#include "dr_aabb_collider.h"
#include "dr_gameObject.h"

namespace driderSDK {
AABBCollider::AABBCollider(GameObject& _gameObject, const AABB& aabb)
: ColliderComponent(_gameObject),
  m_originalAABB(aabb),
  m_transformedAABB(aabb)
{}

const AABB&
AABBCollider::getTransformedAABB() {
  return m_transformedAABB;
}

const AABB&
AABBCollider::getAABB() {
  return m_originalAABB;
}

void
AABBCollider::onCreate() {
}

void 
AABBCollider::onUpdate() {
  if (m_gameObject.changed()) {
    m_transformedAABB = m_originalAABB;
    m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
  }  
}

void 
AABBCollider::onDestroy() {
}

COLLIDER_TYPE::E 
AABBCollider::getType() {
  return COLLIDER_TYPE::kAABB;
}
}