#include "SkeletonDebug.h"
#include <dr_aabb.h>
#include <dr_animator.h>
#include <dr_skeleton.h>
#include <dr_transform.h>

namespace driderSDK {

SkeletonDebug::SkeletonDebug(GameObject& _go, 
                             const Skeleton& skeleton, 
                             Animator& anim,
                             Transform& _transform)
  : GameComponent(_go),
    m_transform(_transform),
    m_skeleton(skeleton),
    m_anim(anim)

{}

void 
SkeletonDebug::create() {
}

void SkeletonDebug::setShaderTechnique(Technique* technique) {
  for (auto& aabb : m_aabbs) {
    aabb->setShaderTechnique(technique);
  }
  m_globalAABB->setShaderTechnique(technique);
}

void
SkeletonDebug::onCreate() {
  
  for (auto& aabb : m_skeleton.bonesAABBs) {
    m_aabbs.push_back(dr_make_unique<AABBDebug>(m_gameObject));
  }

  m_globalAABB = dr_make_unique<AABBDebug>(m_gameObject);
  m_globalAABB->onCreate();

  for (auto& aabb : m_aabbs) {
    aabb->onCreate();
  }
}

void 
SkeletonDebug::onUpdate() {
  
  Int32 index = 0;

  auto transf = m_anim.getTransforms();

  auto aabbs = m_skeleton.bonesAABBs;
  
  index = 0;

  for (auto& aabb : aabbs) {
    aabb.recalculate(transf[index].transpose());
    aabb.recalculate(m_transform.getMatrix());
    m_aabbs[index]->setAABB(aabb);
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

  AABB a{diff.x, diff.y, diff.z, (max + min) * 0.5f};
  //a.recalculate();

  m_globalAABB->setAABB(a);
}

void 
SkeletonDebug::onRender() {
  for (auto& aabb : m_aabbs) {    
    aabb->onRender();
  }
  m_globalAABB->onRender();
}

void 
SkeletonDebug::onDestroy() {
  for (auto& aabb : m_aabbs) {
    aabb->onDestroy();
  }
  m_globalAABB->onDestroy();
}


}