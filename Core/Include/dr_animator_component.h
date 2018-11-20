#pragma once

#include <vector>
#include <unordered_map>

#include <dr_id_object.h>
#include <dr_memory.h>
#include <dr_quaternion.h>

#include "dr_animation.h"
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include "dr_skeleton.h"

namespace driderSDK {

class Matrix4x4;

/**
* Component used to manage a gameObject animations
*/

class DR_CORE_EXPORT AnimatorComponent : public GameComponent,
                                         public IDClass<AnimatorComponent>
{

 public:

  using SharedAnimation = std::shared_ptr<Animation>;
  using SharedSkeleton = std::shared_ptr<Skeleton>;
  using TransformsList = std::vector<Matrix4x4>;
  using FrameCache = std::vector<std::tuple<UInt32, UInt32, UInt32>>;

  AnimatorComponent(GameObject& _gameObject);

  void
  addAnimation(SharedAnimation animation,  
               const TString& animName);

  void
  setCurrentAnimation(const TString& animName,
                      bool reuseElapsedTime);

  void
  blendAnimation(const TString& animName, 
                 bool cloneElpasedTime);

  //Use to combine animations
  void 
  mergeAnimation(const TString& animName, 
                 float alpha, 
                 bool cloneElapsedTime);

  void
  isolateAnimation();

  void
  setSkeleton(SharedSkeleton skeleton);

  void
  setTime(float time);

  void
  setBlendDuration(float blendDur);

  void 
  setSpeed(float speed);

  float 
  getSpeed() const;

  bool
  isBlending() const;

  float 
  getBlendDuration() const;

  SharedSkeleton
  getSkeleton() const;

  SharedAnimation
  getCurrentAnimation() const;

  SharedAnimation
  getNextAnimation() const;

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

  virtual UInt32
  getClassID() override;

  virtual GameComponent*
  cloneIn(GameObject& _go) override;

  Quaternion
  interpolateRotation(const Animation::BoneAnim& boneAnim,
                      float animationTime,
                      const Animation& animation,
                      FrameCache& frameCache);

  Vector3D
  interpolateTranslation(const Animation::BoneAnim& boneAnim,
                         float animationTime,
                         const Animation& animation,
                         FrameCache& frameCache);

  Vector3D
  interpolateScale(const Animation::BoneAnim& boneAnim,
                   float animationTime,
                   const Animation& animation,
                   FrameCache& frameCache);


  template<UInt32 index, class T> UInt32
  getTransformIndex(const Animation::AnimTransformations<T>& animTransforms,
                    float animationTime, 
                    FrameCache& frameCache)
  {
      UInt32 frame = 0;
      
      DR_ASSERT(!animTransforms.empty());

      if (animTransforms.size() > 1) {
        if (animationTime >= *m_lastTimeRef) {
          frame = std::get<index>(frameCache[m_currentBone]);
        }

        while ( frame < static_cast<UInt32>(animTransforms.size() - 1)) {

          if (animationTime < animTransforms[frame + 1].time) {
            break;
          }
            
          ++frame;
        }
      }

      std::get<index>(frameCache[m_currentBone]) = frame;
    
      return frame;                                    
  }

  void
  readNodeHeirarchy(float animTime,
                    float animTimeNext,
                    Skeleton::NodeData* node, 
                    const Matrix4x4& parentTransform,
                    const Animation& animation,
                    const Animation* nextAnim,
                    const Skeleton& skeleton);

 private:

  using WeakAnimation = std::weak_ptr<Animation>;
  using AnimationsMap = std::unordered_map<TString, WeakAnimation>;
  using WeakSkeleton = std::weak_ptr<Skeleton>;

  FrameCache m_lastPositions;
  FrameCache m_lastPosNext;
  UInt32 m_currentBone;
  float m_lastTime;
  float m_lastTimeNext;
  float* m_lastTimeRef;
  float m_elapsed;
  float m_elapsedNext;
  bool m_blending;
  float m_blendVal;
  float m_blendDuration;
  float m_speed;
  bool m_animMerge;
  TransformsList m_transforms;
  WeakSkeleton m_skeleton;
  WeakAnimation m_currentAnim;
  WeakAnimation m_nextAnim;
  AnimationsMap m_animations;
};

}

