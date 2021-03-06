#include "dr_animator_component.h"

#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_matrix4x4.h>
#include <dr_time.h>

#include "dr_animation.h"
#include "dr_skeleton.h"

namespace driderSDK {

AnimatorComponent::AnimatorComponent(GameObject& _gameObject) 
 :  GameComponent(_gameObject, _T("AnimatorComponent")),
    m_blending(false),
    m_blendVal(0.0f),
    m_lastTimeRef(&m_lastTime),
    m_blendDuration(1.0f)
{}

void
AnimatorComponent::addAnimation(SharedAnimation animation, 
                                const TString& animName) {
  m_animations[animName] = animation;
}

void 
AnimatorComponent::setCurrentAnimation(const TString& animName, 
                                       bool blend,
                                       bool cloneElapsedTime) {
  
  auto it = m_animations.find(animName);
  
  if (it != m_animations.end()) {
    /*m_currentAnim = it->second;

    auto anim = getCurrentAnimation();

    if (anim) {
      m_lastPositions = FrameCache(anim->getBonesAnimations().size(),
                                   std::make_tuple(0,0,0));
    }*/
    
    auto currAnim = getCurrentAnimation();

    //If there is an animation already running
    if (currAnim && blend) {

      //If there are 2 animationns blending
      if (getNextAnimation()) {
        
        m_nextAnim = it->second;

        m_currentAnim = m_nextAnim;

        if (auto curr = getCurrentAnimation()) {
          m_lastPositions = FrameCache(curr->getBonesAnimations().size(),
                                       std::make_tuple(0,0,0));
        }

      }
      else {
        m_nextAnim = it->second;
      }
    }
    else {
      m_currentAnim = it->second;

      //If the animation isn't changed the chache will be cleared for
      //current animation
      if (auto curr = getCurrentAnimation()) {
        m_lastPositions = FrameCache(curr->getBonesAnimations().size(),
                                         std::make_tuple(0,0,0));
      }
    }


    if (auto next = getNextAnimation()) {
      m_lastPosNext = FrameCache(next->getBonesAnimations().size(),
                                  std::make_tuple(0,0,0));
      m_blending = true;
      m_blendVal = 0.0f;
      if (cloneElapsedTime) {
        m_elapsedNext = m_elapsed;
      }
      else {
        m_elapsedNext = 0.0f;
      }
      m_lastTimeNext  = 0.0f;
    }
        
  }
}

void 
AnimatorComponent::setSkeleton(SharedSkeleton skeleton) {
  m_skeleton = skeleton;

  if (skeleton) {
    m_transforms = std::vector<Matrix4x4>(skeleton->bones.size(), 
                                          Matrix4x4::identityMat4x4);
  }  
}

void
AnimatorComponent::setTime(float time) {
  m_elapsed = time;
}

void 
AnimatorComponent::setBlendDuration(float blendDur) {
  m_blendDuration = blendDur;
}

AnimatorComponent::SharedSkeleton 
AnimatorComponent::getSkeleton() const {
  return m_skeleton.lock();
}

AnimatorComponent::SharedAnimation 
AnimatorComponent::getCurrentAnimation() const {
  return m_currentAnim.lock();
}

AnimatorComponent::SharedAnimation
AnimatorComponent::getNextAnimation() const {
  return m_nextAnim.lock();
}

const AnimatorComponent::TransformsList&
AnimatorComponent::getBonesTransforms() const {
  return m_transforms;
}

void 
AnimatorComponent::onCreate() {
  m_elapsed = 0;
}

void 
AnimatorComponent::onUpdate() {


  if (m_blending) {
    //Advance 0.25f per second so blend will be completed in 4 seconds
    
    m_blendVal += (1.f / m_blendDuration) * Time::getDelta();
    m_elapsedNext += Time::getDelta();

    if (m_blendVal >= 1.0f) {
      m_blending = false;
      m_currentAnim = m_nextAnim;
      m_nextAnim = WeakAnimation();
      setTime(m_elapsedNext);
    }
  }

  m_elapsed += Time::getDelta();

  auto skeleton = getSkeleton();
  auto animation = getCurrentAnimation();

  if (skeleton && animation) {
    
    float tps = animation->getTicksPerSecond();
    float duration = animation->getDurationInTicks();
    float timeInTicks = m_elapsed * tps;

    DR_ASSERT(duration);

    float animTime = std::fmod(timeInTicks, duration);
    
    float animTimeNext = 0.0f;

    Animation* nextAnimP = nullptr;

    if (m_blending) {
      
      auto nextAnim = getNextAnimation();

      nextAnimP = nextAnim.get();

      float tpsN = nextAnim->getTicksPerSecond();
      float durationN = nextAnim->getDurationInTicks();
      float timeInTicksN = m_elapsedNext * tpsN;

      animTimeNext = std::fmod(timeInTicksN, durationN);
    }

    readNodeHeirarchy(animTime, 
                      animTimeNext,
                      skeleton->root.get(), 
                      Matrix4x4::identityMat4x4, 
                      *animation,
                      nextAnimP,
                      *skeleton);
  
    m_lastTimeNext = animTimeNext;
    m_lastTime = animTime;

    for (SizeT i = 0; i < skeleton->bones.size(); ++i) {
      m_transforms[i] = skeleton->bones[i]->finalTransform;
    }
  }
  else {

    if (!animation) {
      Logger::addLog(_T("Animator: animation not assigned to ") + 
                     m_gameObject.getName());
    }

    if (!skeleton) {
      Logger::addLog(_T("Animator: skeleton not assigned to ") + 
                     m_gameObject.getName());
    }

  }
}

void  
AnimatorComponent::onRender() {

}

void 
AnimatorComponent::onDestroy() {

}

void
AnimatorComponent::cloneIn(GameObject& _go) {
  
  auto dup = _go.createComponent<AnimatorComponent>();

  dup->m_lastPositions  = m_lastPositions;
  dup->m_currentBone    = m_currentBone;
  dup->m_lastTime       = m_lastTime;
  dup->m_transforms     = m_transforms;
  dup->m_skeleton       = m_skeleton;
  dup->m_currentAnim    = m_currentAnim;
  dup->m_animations     = m_animations; 
  dup->m_elapsed        = m_elapsed;
  dup->m_elapsedNext    = m_elapsedNext;
  dup->m_lastPosNext    = m_lastPosNext;
  dup->m_blendVal       = m_blendVal;
  dup->m_nextAnim       = m_nextAnim;
  dup->m_lastTimeNext   = m_lastTimeNext;
}

Quaternion 
AnimatorComponent::interpolateRotation(const Animation::BoneAnim& boneAnim, 
                                       float animationTime, 
                                       const Animation& animation,
                                       FrameCache& frameCache) {
  using RotationAnimFrame = Animation::AnimationTransform<Quaternion>;

  UInt32 frame = getTransformIndex<2>(boneAnim.rotations, 
                                      animationTime, 
                                      frameCache);

  UInt32 nextFrame = (frame + 1) % boneAnim.rotations.size();

  const RotationAnimFrame& currentRot = boneAnim.rotations[frame];

  Quaternion interpolatedRot;

  if (frame != nextFrame) {
    
    const RotationAnimFrame& nextRot = boneAnim.rotations[nextFrame];

    float timeBtwnFrames = nextRot.time - currentRot.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += animation.getDurationInTicks();
    } 

    float factor = (animationTime - currentRot.time) / timeBtwnFrames;

    interpolatedRot = currentRot.value.slerp(nextRot.value, factor);
  } 
  else {
    interpolatedRot = currentRot.value;
  }

  return interpolatedRot;
}

Vector3D 
AnimatorComponent::interpolateTranslation(const Animation::BoneAnim& boneAnim,
                                          float animationTime, 
                                          const Animation& animation,
                                          FrameCache& frameCache) {
  using TransitionAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<0>(boneAnim.positions, 
                                      animationTime,
                                      frameCache);

  UInt32 nextFrame = (frame + 1) % boneAnim.positions.size();

  const TransitionAnimFrame& currentTrans = boneAnim.positions[frame];

  Vector3D interpolatedTrans;

  if (frame != nextFrame) {
    
    const TransitionAnimFrame& nextTrans = boneAnim.positions[nextFrame];

    float timeBtwnFrames = nextTrans.time - currentTrans.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += animation.getDurationInTicks();
    } 

    float factor = (animationTime - currentTrans.time) / timeBtwnFrames;

    interpolatedTrans = currentTrans.value + 
                        (nextTrans.value - currentTrans.value) * factor;
  } 
  else {
    interpolatedTrans = currentTrans.value;
  }

  return interpolatedTrans;
}

Vector3D 
AnimatorComponent::interpolateScale(const Animation::BoneAnim& boneAnim, 
                                    float animationTime,
                                    const Animation& animation,
                                    FrameCache& frameCache) {
  using ScaleAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<1>(boneAnim.scales, 
                                      animationTime,
                                      frameCache);

  UInt32 nextFrame = (frame + 1) % boneAnim.scales.size();

  const ScaleAnimFrame& currentScale = boneAnim.scales[frame];

  Vector3D interpolatedScale;

  if (frame != nextFrame) {
    
    const ScaleAnimFrame& nextScale = boneAnim.scales[nextFrame];

    float timeBtwnFrames = nextScale.time - currentScale.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += animation.getDurationInTicks();
    } 

    float factor = (animationTime - currentScale.time) / timeBtwnFrames;

    interpolatedScale = currentScale.value + 
                        (nextScale.value - currentScale.value) * factor;
  } 
  else {
    interpolatedScale = currentScale.value;
  }

  return interpolatedScale;
}

void 
AnimatorComponent::readNodeHeirarchy(float animTime, 
                                     float animTimeNext,
                                     Skeleton::NodeData* node, 
                                     const Matrix4x4& parentTransform, 
                                     const Animation& animation, 
                                     const Animation* nextAnim,
                                     const Skeleton& skeleton) {

  Matrix4x4 nodeTransform = node->transform;

  auto pBoneAnim = animation.getBoneAnimation(node->name);

  if (pBoneAnim) {
        
    m_currentBone = animation.getBoneIndex(node->name);

    m_lastTimeRef = &m_lastTime;

    Vector3D translation = interpolateTranslation(*pBoneAnim, 
                                                  animTime, 
                                                  animation, 
                                                  m_lastPositions);

    Quaternion rotation = interpolateRotation(*pBoneAnim,
                                              animTime, 
                                              animation, 
                                              m_lastPositions);
    
    Vector3D scaling = interpolateScale(*pBoneAnim,
                                         animTime, 
                                         animation,
                                         m_lastPositions);

    if (m_blending) {

      if (auto pBoneNext = nextAnim->getBoneAnimation(node->name)) {

        m_currentBone = nextAnim->getBoneIndex(node->name);

        m_lastTimeRef = &m_lastTimeNext;

        Vector3D nTranslation = interpolateTranslation(*pBoneNext, 
                                                  animTimeNext, 
                                                  *nextAnim, 
                                                  m_lastPosNext);

        Quaternion nRotation = interpolateRotation(*pBoneNext,
                                                  animTimeNext, 
                                                  *nextAnim, 
                                                  m_lastPosNext);
    
        Vector3D nScaling = interpolateScale(*pBoneNext,
                                             animTimeNext, 
                                             *nextAnim,
                                             m_lastPosNext);

        translation = translation + (nTranslation - translation) * m_blendVal;
        rotation = rotation.slerp(nRotation, m_blendVal);
        scaling = scaling + (nScaling - scaling) * m_blendVal;
      }
    }
    
    rotation.matrixFromQuaternion(nodeTransform);
    nodeTransform.Scale(scaling);
    nodeTransform.Translation(translation);
  }
  else {
    nodeTransform.transpose();
  }

  Matrix4x4 globalTransform = nodeTransform * parentTransform;

  auto boneIt = skeleton.bonesMapping.find(node->name);

  if (boneIt != skeleton.bonesMapping.end()) {
    
    auto& pBone = skeleton.bones[boneIt->second];

    pBone->finalTransform = //skeleton.gloabalInverseTransform * 
                            pBone->boneOffset * globalTransform;

    /*m_transforms[boneIt->second] = skeleton.bones[boneIt->second]->boneOffset * 
                                   globalTransform;*/
  }

  for (auto& pChild : node->children) {
    readNodeHeirarchy(animTime, 
                      animTimeNext,
                      pChild.get(), 
                      globalTransform, 
                      animation,
                      nextAnim,
                      skeleton);
  }
}

}