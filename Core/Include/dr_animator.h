#pragma once

#include <tuple>
#include <vector>
#include <dr_matrix4x4.h>
#include "dr_animation.h"
#include "dr_skeleton.h"

#include <map>

namespace driderSDK {

class Skeleton;

class DR_CORE_EXPORT Animator 
{
 public:

  Animator() 
  : m_lastTime(0.f)
  {}

  void 
  setAnimation(std::shared_ptr<Animation> pAnimation);

  void 
  setSkeleton(std::shared_ptr<Skeleton> pSkeleton);
  
  void 
  evaluate(float time);

  const std::vector<Matrix4x4>&
  getTransforms()
  {
    return m_transforms;
  }

 private:

  Quaternion
  interpolateRotation(const Animation::BoneAnim& boneAnim,
                      float animationTime,
                      const Animation* pAnimation);

  Vector3D
  interpolateTranslation(const Animation::BoneAnim& boneAnim,
                         float animationTime,
                         const Animation* pAnimation);

  Vector3D
  interpolateScale(const Animation::BoneAnim& boneAnim,
                   float animationTime,
                   const Animation* pAnimation);



  template<UInt32 index, class T> UInt32
  getTransformIndex(const Animation::AnimTransformations<T>& animTransforms,
                    float animationTime)
  {
      UInt32 frame = 0;
      
      DR_ASSERT(!animTransforms.empty());

      if (animTransforms.size() > 1) {
        if (animationTime >= m_lastTime) {
          frame = std::get<index>(m_lastPositions[m_currentBone]);
        }

        while ( frame < static_cast<UInt32>(animTransforms.size() - 1)) {

          if (animationTime < animTransforms[frame + 1].time) {
            break;
          }
            
          ++frame;
        }

      }

      return frame;                                    
  }

private:

  void
  readNodeHeirarchy(float animTime,
                    Skeleton::NodeData* pNode, 
                    const Matrix4x4& parentTransform,
                    const Animation* pAnimation,
                    const Skeleton* pSkeleton, Int32 level);

  std::vector<std::tuple<UInt32, UInt32, UInt32>> m_lastPositions;

  float m_lastTime;

  UInt32 m_currentBone;

  std::weak_ptr<Skeleton> m_pSkeleton;

  std::weak_ptr<Animation> m_pAnimation;

  std::vector<Matrix4x4> m_transforms;
};

}