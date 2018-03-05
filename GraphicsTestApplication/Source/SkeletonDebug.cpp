#include "SkeletonDebug.h"
#include <dr_aabb.h>
#include <dr_animator_component.h>
#include <dr_gameObject.h>
#include <dr_skeleton.h>
#include <dr_transform.h>

namespace driderSDK {

SkeletonDebug::SkeletonDebug(GameObject& _go)
  : GameComponent(_go)
{}

void 
SkeletonDebug::create() {
}

void SkeletonDebug::setShaderTechnique(Technique* technique) {
  for (auto& aabb : m_aabbs) {
    aabb->setShaderTechnique(technique);
  }

  m_technique = technique;
}

void
SkeletonDebug::onCreate() {
  
  auto anim = m_gameObject.getComponent<AnimatorComponent>();

  for (auto& aabb : anim->getSkeleton()->bonesAABBs) {
    m_aabbs.push_back(dr_make_unique<AABBDebug>(m_gameObject));
  }


  for (auto& aabb : m_aabbs) {
    aabb->onCreate();
  }
}

void 
SkeletonDebug::onUpdate() {
  
  Int32 index = 0;

  auto anim = m_gameObject.getComponent<AnimatorComponent>();

  auto transf = anim->getBonesTransforms();

  auto aabbs = anim->getSkeleton()->bonesAABBs;
  
  index = 0;

  for (auto& aabb : aabbs) {
    aabb.recalculate(transf[index].transpose());
    aabb.recalculate(m_gameObject.getWorldTransform().getMatrix());
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
}

void 
SkeletonDebug::onRender() {
  for (auto& aabb : m_aabbs) {    
    aabb->onRender();
  }
}

void 
SkeletonDebug::onDestroy() {
  for (auto& aabb : m_aabbs) {
    aabb->onDestroy();
  }
}

void 
SkeletonDebug::cloneIn(GameObject& _go) {

  auto dup = _go.createComponent<SkeletonDebug>();
  dup->setShaderTechnique(m_technique);
}


}