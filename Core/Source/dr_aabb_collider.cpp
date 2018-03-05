#include "dr_aabb_collider.h"
#include "dr_animator_component.h"
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

  //Calculate AABB using animation
  if (auto anim = m_gameObject.getComponent<AnimatorComponent>()) {
    
    auto& transf = anim->getBonesTransforms();

    auto aabbs = anim->getSkeleton()->bonesAABBs;
  
    Int32 index = 0;

    for (auto& aabb : aabbs) {
      aabb.recalculate(transf[index]);
      ++index;
    }

    Vector3D min{Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT};
    Vector3D max{Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT};

    for (auto& aabb : aabbs) {
      Vector3D lmin = aabb.getMinPoint();
      Vector3D lmax = aabb.getMaxPoint();
      for (Int32 i = 0; i < 3; ++i) {
        min[i] = Math::min(lmin[i], min[i]);
        max[i] = Math::max(lmax[i], max[i]);
      }
    }

    Vector3D diff = max - min;

    m_transformedAABB = AABB{diff.x, diff.y, diff.z, (max + min) * 0.5f};
    m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
  }
  else {
    if (m_gameObject.changed()) {
      m_transformedAABB = m_originalAABB;
      m_transformedAABB.recalculate(m_gameObject.getWorldTransform().getMatrix());
    }  
  }

  
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