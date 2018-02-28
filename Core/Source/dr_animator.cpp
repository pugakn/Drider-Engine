#include "dr_animator.h"
#include <dr_string_utils.h>

namespace driderSDK {
void 
Animator::setAnimation(std::shared_ptr<Animation> pAnimation) {
  m_pAnimation = pAnimation;

  m_lastTime = 0.0f;

  if (auto pAnim = m_pAnimation.lock()) {
    m_lastPositions.resize(pAnim->getBonesAnimations().size(), 
                           std::make_tuple(0, 0, 0));
  }
}

void 
Animator::setSkeleton(std::shared_ptr<Skeleton> pSkeleton) {
  m_pSkeleton = pSkeleton;
}

Quaternion 
Animator::interpolateRotation(const Animation::BoneAnim& boneAnim, 
                              float animationTime, 
                              const Animation* pAnimation) {

  using RotationAnimFrame = Animation::AnimationTransform<Quaternion>;

  UInt32 frame = getTransformIndex<2>(boneAnim.rotations, animationTime);

  UInt32 nextFrame = (frame + 1) % boneAnim.rotations.size();

  const RotationAnimFrame& currentRot = boneAnim.rotations[frame];

  Quaternion interpolatedRot;

  if (frame != nextFrame) {
    
    const RotationAnimFrame& nextRot = boneAnim.rotations[nextFrame];

    float timeBtwnFrames = nextRot.time - currentRot.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += pAnimation->getDurationInTicks();
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
Animator::interpolateTranslation(const Animation::BoneAnim& boneAnim, 
                                 float animationTime, 
                                 const Animation* pAnimation) {
  using TransitionAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<0>(boneAnim.positions, animationTime);

  UInt32 nextFrame = (frame + 1) % boneAnim.positions.size();

  const TransitionAnimFrame& currentTrans = boneAnim.positions[frame];

  Vector3D interpolatedTrans;

  if (frame != nextFrame) {
    
    const TransitionAnimFrame& nextTrans = boneAnim.positions[nextFrame];

    float timeBtwnFrames = nextTrans.time - currentTrans.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += pAnimation->getDurationInTicks();
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
Animator::interpolateScale(const Animation::BoneAnim& boneAnim, 
                           float animationTime, 
                           const Animation* pAnimation) {

  using ScaleAnimFrame = Animation::AnimationTransform<Vector3D>;

  UInt32 frame = getTransformIndex<1>(boneAnim.scales, animationTime);

  UInt32 nextFrame = (frame + 1) % boneAnim.scales.size();

  const ScaleAnimFrame& currentScale = boneAnim.scales[frame];

  Vector3D interpolatedScale;

  if (frame != nextFrame) {
    
    const ScaleAnimFrame& nextScale = boneAnim.scales[nextFrame];

    float timeBtwnFrames = nextScale.time - currentScale.time;
    
    if (nextFrame < frame) {
      timeBtwnFrames += pAnimation->getDurationInTicks();
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
Animator::evaluate(float time) {
  
  auto pSkeleton = m_pSkeleton.lock();
  auto pAnimation = m_pAnimation.lock();

  DR_ASSERT(pSkeleton && pAnimation);
  
  float tps = pAnimation->getTicksPerSecond();
  float duration = pAnimation->getDurationInTicks();
  float timeInTicks = time * tps;

  DR_ASSERT(duration);

  float animTime = std::fmod(timeInTicks, duration);

  if (m_transforms.size() != pSkeleton->bones.size()) {
    m_transforms.resize(pSkeleton->bones.size());
  }

  readNodeHeirarchy(animTime, 
                    pSkeleton->pRoot.get(), 
                    Matrix4x4::identityMat4x4, 
                    pAnimation.get(),
                    pSkeleton.get(), 0);
  
  m_lastTime = animTime;

  for (SizeT i = 0; i < pSkeleton->bones.size(); ++i) {
    m_transforms[i] = pSkeleton->bones[i]->finalTransform;
  }
}

void 
Animator::readNodeHeirarchy(float animTime, 
                            Skeleton::NodeData* pNode, 
                            const Matrix4x4 & parentTransform,
                            const Animation* pAnimation,
                            const Skeleton* pSkeleton, Int32 level) {

  Matrix4x4 nodeTransform = pNode->transform;

  auto pBoneAnim = pAnimation->getBoneAnimation(pNode->name);

  if (pBoneAnim) {
        
    m_currentBone = pAnimation->getBoneIndex(pNode->name);

    Matrix4x4 rotatMatrix(Math::kIdentity);
    Matrix4x4 transMatrix(Math::kIdentity);
    Matrix4x4 scaleMatrix(Math::kIdentity);

    Vector3D translation = interpolateTranslation(*pBoneAnim, 
                                                  animTime, 
                                                  pAnimation);

    Quaternion rotation = interpolateRotation(*pBoneAnim,
                         animTime, 
                         pAnimation);
    
    Vector3D scaling = interpolateScale(*pBoneAnim,
                                         animTime, 
                                         pAnimation);
    
    transMatrix.Translation(translation);
    rotation.matrixFromQuaternion(rotatMatrix);
    scaleMatrix.Scale(scaling);
    nodeTransform = rotatMatrix * scaleMatrix;    
    nodeTransform[0][3] =  translation.x;    
    nodeTransform[1][3] = translation.y;     
    nodeTransform[2][3] = translation.z;
  }

  Matrix4x4 globalTransform = parentTransform * nodeTransform;

  auto boneIt = pSkeleton->bonesMapping.find(pNode->name);

  if (boneIt != pSkeleton->bonesMapping.end()) {
    auto& pBone = pSkeleton->bones[boneIt->second];

    pBone->finalTransform = pSkeleton->gloabalInverseTransform * 
                            globalTransform * 
                            pBone->boneOffset;
  }

  for (auto& pChild : pNode->children) {
    readNodeHeirarchy(animTime, 
                      pChild.get(), 
                      globalTransform, 
                      pAnimation,
                      pSkeleton, level + 1);
  }

}

}


