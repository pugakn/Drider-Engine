#include "dr_animator_component.h"

#include <iostream>

#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_matrix4x4.h>
#include <dr_scoped_timer.h>
#include <dr_time.h>

#include "dr_animation.h"
#include "dr_skeleton.h"

namespace driderSDK {

AnimatorComponent::AnimatorComponent(GameObject& _gameObject) 
 :  GameComponent(_gameObject, _T("AnimatorComponent")),
    m_blending(false),
    m_blendVal(0.0f),
    m_lastTimeRef(&m_lastTime),
    m_blendDuration(1.0f),
    m_speed(1.0f),
    m_animMerge(false)
{}

void
AnimatorComponent::addAnimation(SharedAnimation animation, 
                                const TString& animName) {
  m_animations[animName] = animation;
}

void 
AnimatorComponent::setCurrentAnimation(const TString& animName, 
                                       bool cloneElapsedTime) {
  
  auto it = m_animations.find(animName);
  
  if (it != m_animations.end()) {

    m_nextAnim = WeakAnimation();

    m_blending = false;

    m_animMerge = false;
     
    m_currentAnim = it->second;

    //Start from 0
    if (!cloneElapsedTime) {
      m_elapsed = 0;
      m_lastTime = 0.0f;
    }

    if (auto curr = getCurrentAnimation()) {
      m_lastPositions = FrameCache(curr->getBonesAnimations().size(),
                                   std::make_tuple(0,0,0));
    }
    else {
      throw std::exception("Requested animation no longer exists");
    } 
  }
  else {
    throw std::exception("Requested animation isn't loaded");
  }
}

void
AnimatorComponent::blendAnimation(const TString& animName, 
                                  bool cloneElpasedTime) {
  auto it = m_animations.find(animName);
  
  if (it != m_animations.end()) {
    {
      auto next = getNextAnimation();

      if (next && !m_animMerge) {

        m_currentAnim = next;

        m_lastPositions = FrameCache(next->getBonesAnimations().size(),
                                     std::make_tuple(0,0,0));
      
        m_nextAnim = it->second;
          
        m_elapsed = m_elapsedNext;

        m_lastTime = m_lastTimeNext;
      }
      else {
        m_nextAnim = it->second;
      }
    }
    if (auto next = getNextAnimation()) {
      m_lastPosNext = FrameCache(next->getBonesAnimations().size(),
                                 std::make_tuple(0,0,0));
    }
    else {
      throw std::exception("Requested animation no longer exists");
    }
    
    m_animMerge = false;

    m_blending = true;

    m_blendVal = 0.0f;
      
    if (cloneElpasedTime) {
      m_elapsedNext = m_elapsed;
      m_lastTimeNext = m_lastTime;
    }
    else {  
      m_elapsedNext = 0.0f;
      m_lastTimeNext  = 0.0f;  
    }
  }
  else {
    throw std::exception("Requested animation isn't loaded");
  }
}

void 
AnimatorComponent::mergeAnimation(const TString& animName, 
                                  float alpha, 
                                  bool cloneElapsedTime) {

   auto it = m_animations.find(animName);
  
  if (it != m_animations.end()) {
    m_nextAnim = it->second;

    if (auto next = getNextAnimation()) {
      m_lastPosNext = FrameCache(next->getBonesAnimations().size(),
                                 std::make_tuple(0,0,0));
    }
    else {
      throw std::exception("Requested animation no longer exists");
    }

    m_animMerge = true;
    m_blending = true;
    m_blendVal = alpha; 

    if (cloneElapsedTime) {
      m_elapsedNext = m_elapsed;
      m_lastTimeNext = m_lastTime;
    }
    else {  
      m_elapsedNext = 0.0f;
      m_lastTimeNext  = 0.0f;  
    }
  }
  else {
    throw std::exception("Requested animation isn't loaded");
  }  
}

void 
AnimatorComponent::isolateAnimation() {

  if (m_animMerge) {
    m_animMerge = false;
    m_blending = false;

    m_nextAnim = WeakAnimation();
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

void 
AnimatorComponent::setSpeed(float speed) {
  m_speed = speed;
}

float 
AnimatorComponent::getSpeed() const {
  return m_speed;
}

bool
AnimatorComponent::isBlending() const {
  return m_blending;
}

float 
AnimatorComponent::getBlendDuration() const {
  return m_blendDuration;
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

  static int count = 0;
  static float accum = 0.0f;

  Timer timer;
  timer.init();

  if (m_blending) {
    //Advance 0.25f per second so blend will be completed in 4 seconds
    if (!m_animMerge) {
      m_blendVal += (1.f / m_blendDuration) * Time::getDelta() * m_speed;
    }

    m_elapsedNext += Time::getDelta() * m_speed;

    if (m_blendVal >= 1.0f) {
      m_blending = false;
      
      m_currentAnim = m_nextAnim;

      m_nextAnim = WeakAnimation();
      

      if (auto curr = getCurrentAnimation()) {
        m_lastPositions = FrameCache(curr->getBonesAnimations().size(),
                                           std::make_tuple(0,0,0));
      }

      m_elapsed = m_elapsedNext;
      m_lastTime = m_lastTimeNext;
    }
  }

  m_elapsed += Time::getDelta() * m_speed;

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
      
      if (auto nextAnim = getNextAnimation()) {
        nextAnimP = nextAnim.get();

        float tpsN = nextAnim->getTicksPerSecond(); 
        float durationN = nextAnim->getDurationInTicks();
        float timeInTicksN = m_elapsedNext * tpsN;

        animTimeNext = std::fmod(timeInTicksN, durationN);
      }
      else {
        m_blending = false;
        throw std::exception("Animation was erased while begin used");
      }      
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

  accum += timer.getMilliseconds();
  count++;

  if (count == 100) {
    float t = accum / 100;
    std::cout << "Prom: " << t << std::endl;
    count = 0;
    accum = 0;
  }
}

void  
AnimatorComponent::onRender() {

}

void 
AnimatorComponent::onDestroy() {

}

UInt32
AnimatorComponent::getClassID() {
  return CLASS_NAME_ID(AnimatorComponent);
}

GameComponent*
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
  dup->m_speed          = m_speed;
  dup->m_blending       = m_blending;
  dup->m_blendDuration  = m_blendDuration;
  dup->m_animMerge      = m_animMerge;
  dup->m_lastTimeRef    = nullptr;

  return dup;
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
    
    nodeTransform.InitScale(scaling);
    nodeTransform *= rotation.matrixFromQuaternion();
    nodeTransform.Translation(translation);
  }
  else {

  }

  Matrix4x4 globalTransform = nodeTransform * parentTransform;

  auto boneIt = skeleton.bonesMapping.find(node->name);

  if (boneIt != skeleton.bonesMapping.end()) {
    
    auto& pBone = skeleton.bones[boneIt->second];

    pBone->finalTransform = skeleton.gloabalInverseTransform * 
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