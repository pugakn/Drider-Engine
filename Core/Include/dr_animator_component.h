#pragma once

#include <vector>
#include <unordered_map>

#include <dr_memory.h>
#include <dr_quaternion.h>

#include "dr_animation.h"
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include "dr_skeleton.h"

namespace driderSDK {

class Matrix4x4;

class AnimatorComponent : public GameComponent
{

public:

  using SharedAnimation = std::shared_ptr<Animation>;
  using SharedSkeleton = std::shared_ptr<Skeleton>;
  using TransformsList = std::vector<Matrix4x4>;

  using GameComponent::GameComponent;

  void
  addAnimation(SharedAnimation animation,  
               const TString& animName);

  void
  setCurrentAnimation(const TString& animName);

  void
  setSkeleton(SharedSkeleton skeleton);

  SharedSkeleton
  getSkeleton() const;

  SharedAnimation
  getCurrentAnimation() const;

  const TransformsList&
  getBonesTransforms() const;
private:
  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void
  onRender() override;

  virtual void 
  onDestroy() override;

  Quaternion
  interpolateRotation(const Animation::BoneAnim& boneAnim,
                      float animationTime,
                      const Animation& animation);

  Vector3D
  interpolateTranslation(const Animation::BoneAnim& boneAnim,
                         float animationTime,
                      const Animation& animation);

  Vector3D
  interpolateScale(const Animation::BoneAnim& boneAnim,
                   float animationTime,
                      const Animation& animation);

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

      std::get<index>(m_lastPositions[m_currentBone]) = frame;
    
      return frame;                                    
  }

  void
  readNodeHeirarchy(float animTime,
                    Skeleton::NodeData* node, 
                    const Matrix4x4& parentTransform,
                    const Animation& animation,
                    const Skeleton& skeleton);

private:

  using WeakAnimation = std::weak_ptr<Animation>;
  using AnimationsMap = std::unordered_map<TString, WeakAnimation>;
  using WeakSkeleton = std::weak_ptr<Skeleton>;

  
  using FrameCache = std::vector<std::tuple<UInt32, UInt32, UInt32>>;
  
  FrameCache m_lastPositions;
  UInt32 m_currentBone;
  float m_lastTime;
  TransformsList m_transforms;
  WeakSkeleton m_skeleton;
  WeakAnimation m_currentAnim;
  AnimationsMap m_animations;
};

}

