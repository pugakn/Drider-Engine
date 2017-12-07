#pragma once

#include <tuple>
#include <vector>
#include <dr_matrix4x4.h>
#include "dr_animation.h"
#include "dr_skeleton.h"

namespace driderSDK {

class Skeleton;

class Animator 
{
 public:

  Animator() 
  : m_lastTime(0.f)
  {}

  void 
  setAnimation(std::shared_ptr<Animation> pAnimation);

  void 
  setSkeleton(std::shared_ptr<Skeleton> pSkeleton);

 private:

  template<class T> T
  interpolateTransform(const Animation::AnimTransformations<T>& animTransforms,
                       float animationTime, 
                       Int32 transformIndex) 
  {
    static_cast<void>(transformIndex);
  }

  template<class T> UInt32
  getTransformIndex(const Animation::AnimTransformations<T>& animTransforms,
                    float animationTime, 
                    Int32 transformIndex) 
  {
    static_cast<void>(transformIndex);

  }

  void 
  evaluate(float time);

  const std::vector<Matrix4x4>&
  getTransforms()
  {
    return m_transforms;
  }

private:

  void
  readNodeHeirarchy(float animTime,
                    Skeleton::NodeData* pNode, 
                    const Matrix4x4& parentTransform,
                    const Animation* pAnimation,
                    const Skeleton* pSkeleton);

  std::vector<std::tuple<UInt32, UInt32, UInt32>> m_lastPositions;

  float m_lastTime;

  std::weak_ptr<Skeleton> m_pSkeleton;

  std::weak_ptr<Animation> m_pAnimation;

  std::vector<Matrix4x4> m_transforms;
};

}