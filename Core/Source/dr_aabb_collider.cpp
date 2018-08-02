#include "dr_aabb_collider.h"

#include "dr_animator_component.h"
#include "dr_gameObject.h"

namespace driderSDK {
AABBCollider::AABBCollider(GameObject& _gameObject, const AABB& aabb)
: ColliderComponent(_gameObject, _T("AABBCollider")),
  m_originalAABB(aabb),
  m_transformedAABB(aabb)
{}

const AABB&
AABBCollider::getTransformedAABB() {
  return m_transformedAABB;
}

AABB&
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

UInt32 AABBCollider::getClassID()
{
  return CLASS_NAME_ID(AABBCollider);
}

GameComponent*
AABBCollider::cloneIn(GameObject& _go) {
  auto dup = _go.createComponent<AABBCollider>(m_originalAABB);
  dup->m_originalAABB = m_originalAABB;
  dup->m_transformedAABB = m_transformedAABB;
  return dup;
}

COLLIDER_TYPE::E 
AABBCollider::getType() {
  return COLLIDER_TYPE::kAABB;
}
}