#include "dr_animator_component.h"

#include <dr_gameObject.h>
#include <dr_logger.h>
#include <dr_matrix4x4.h>
#include <dr_time.h>

#include "dr_animation.h"
#include "dr_skeleton.h"

namespace driderSDK {

void
AnimatorComponent::addAnimation(SharedAnimation animation, 
                                const TString& animName) {
  m_animations[animName] = animation;
}

void 
AnimatorComponent::setCurrentAnimation(const TString& animName) {
  
  auto it = m_animations.find(animName);
  
  if (it != m_animations.end()) {
    m_currentAnim = it->second;

    auto anim = getCurrentAnimation();

    if (anim) {
      m_lastPositions = FrameCache(anim->getBonesAnimations().size(),
                                   std::make_tuple(0,0,0));
    }
  }
}

void 
AnimatorComponent::setSkeleton(SharedSkeleton skeleton) {
  m_skeleton = skeleton;

  Matrix4x4 identity(Math::FORCE_INIT::kIdentity);

  if (skeleton) {
    m_transforms = std::vector<Matrix4x4>(skeleton->bones.size(), identity);
  }  
}

void
AnimatorComponent::setTime(float time) {
  m_elapsed = time;
}

AnimatorComponent::SharedSkeleton 
AnimatorComponent::getSkeleton() const {
  return m_skeleton.lock();
}

AnimatorComponent::SharedAnimation 
AnimatorComponent::getCurrentAnimation() const {
  return m_currentAnim.lock();
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


  auto skeleton = getSkeleton();
  auto animation = getCurrentAnimation();

  if (skeleton && animation) {

    m_elapsed += Time::getDelta();

    float tps = animation->getTicksPerSecond();
    float duration = animation->getDurationInTicks();
    float timeInTicks = m_elapsed * tps;

    DR_ASSERT(duration);

    float animTime = std::fmod(timeInTicks, duration);
    
    readNodeHeirarchy(animTime, 
                      skeleton->root.get(), 
                      Matrix4x4::identityMat4x4, 
                      *animation,
                      *skeleton);
  
    m_lastTime = animTime;

    for (SizeT i = 0; i < skeleton->bones.size(); ++i) {
      m_transforms[i] = skeleton->bones[i]->finalTransform;
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
}

Quaternion 
AnimatorComponent::interpolateRotation(const Animation::BoneAnim& boneAnim, 
                                       float animationTime, 
                                       const Animation& animation) {
  using RotationAnimFrame = Animation::AnimationTransform<Quaternion>;

  UInt32 frame = getTransformIndex<2>(boneAnim.rotations, animationTime);

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
                                                   const Animation& animation) {
  using TransitionAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<0>(boneAnim.positions, animationTime);

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
                                             const Animation& animation) {
  using ScaleAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<1>(boneAnim.scales, animationTime);

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
                                          Skeleton::NodeData* node, 
                                          const Matrix4x4& parentTransform, 
                                          const Animation& animation, 
                                          const Skeleton& skeleton) {

  Matrix4x4 nodeTransform = node->transform;

  auto pBoneAnim = animation.getBoneAnimation(node->name);

  if (pBoneAnim) {
        
    m_currentBone = animation.getBoneIndex(node->name);

    Matrix4x4 rotatMatrix(Math::kIdentity);
    Matrix4x4 transMatrix(Math::kIdentity);
    Matrix4x4 scaleMatrix(Math::kIdentity);

    Vector3D translation = interpolateTranslation(*pBoneAnim, 
                                                  animTime, 
                                                  animation);

    Quaternion rotation = interpolateRotation(*pBoneAnim,
                         animTime, 
                         animation);
    
    Vector3D scaling = interpolateScale(*pBoneAnim,
                                         animTime, 
                                         animation);
    
    transMatrix.Translation(translation);
    rotation.matrixFromQuaternion(rotatMatrix);
    scaleMatrix.Scale(scaling);
    nodeTransform = rotatMatrix * scaleMatrix;    
    nodeTransform[0][3] =  translation.x;    
    nodeTransform[1][3] = translation.y;     
    nodeTransform[2][3] = translation.z;
  }

  Matrix4x4 globalTransform = parentTransform * nodeTransform;

  auto boneIt = skeleton.bonesMapping.find(node->name);

  if (boneIt != skeleton.bonesMapping.end()) {
    auto& pBone = skeleton.bones[boneIt->second];

    pBone->finalTransform = skeleton.gloabalInverseTransform * 
                            globalTransform * 
                            pBone->boneOffset;
  }

  for (auto& pChild : node->children) {
    readNodeHeirarchy(animTime, 
                      pChild.get(), 
                      globalTransform, 
                      animation,
                      skeleton);
  }
}

}