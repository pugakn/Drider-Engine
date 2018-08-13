#include "dr_aabb_collider.h"
#include "dr_animator_component.h"
#include "dr_gameObject.h"
#include "dr_graph.h"
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

const AABB&
AABBCollider::getAABB() {
  return m_originalAABB;
}

void
AABBCollider::onCreate() {

}

void 
AABBCollider::onUpdate() {

  //Calculate AABB using animation
  auto animator = m_gameObject.getComponent<AnimatorComponent>();

  if (animator && animator->getSkeleton()) {
    
    auto& transf = animator->getBonesTransforms();

    auto aabbs = animator->getSkeleton()->bonesAABBs;

    Vector3D min{Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT};
    Vector3D max{Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT};

    Int32 index = 0;

    for (auto& aabb : aabbs) {

      aabb.recalculate(transf[index]);

      Vector3D lmin = aabb.getMinPoint();
      Vector3D lmax = aabb.getMaxPoint();

      for (Int32 i = 0; i < 3; ++i) {
        min[i] = Math::min(lmin[i], min[i]);
        max[i] = Math::max(lmax[i], max[i]);
      }

      ++index;
    }

    Vector3D diff = max - min;

    m_transformedAABB = AABB{diff.x, diff.y, diff.z, (max + min) * 0.5f};
    m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());      
  }
  else if (m_gameObject.changed()) {
      m_transformedAABB = m_originalAABB;
      m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
  } 

  //Collisions
  ColliderComponent::onUpdate();
}

void 
AABBCollider::onDestroy() {
}

void
AABBCollider::cloneIn(GameObject& _go) {
  auto dup = _go.createComponent<AABBCollider>(m_originalAABB);
  dup->m_transformedAABB = m_transformedAABB;
}

COLLIDER_TYPE::E 
AABBCollider::getType() {
  return COLLIDER_TYPE::kAABB;
}
}