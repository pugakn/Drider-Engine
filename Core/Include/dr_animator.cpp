#include "dr_animator.h"

namespace driderSDK {
void 
Animator::setAnimation(std::shared_ptr<Animation> pAnimation) {
  m_pAnimation = pAnimation;
}

void 
Animator::setSkeleton(std::shared_ptr<Skeleton> pSkeleton) {
  m_pSkeleton = pSkeleton;
}

void Animator::evaluate(float time) {

  auto pSkeleton = m_pSkeleton.lock();
  auto pAnimation = m_pAnimation.lock();

  DR_ASSERT(pSkeleton && pAnimation);
  
  float tps = pAnimation->getTicksPerSecond();
  float duration = pAnimation->getDurationInTicks();
  float timeInTicks = time * tps;

  DR_ASSERT(duration);

  float animTime = std::fmod(timeInTicks, duration);

  if(m_transforms.size() != pSkeleton->bones.size()) {
    m_transforms.resize(pSkeleton->bones.size());
  }

  readNodeHeirarchy(animTime, 
                    pSkeleton->pRoot.get(), 
                    Matrix4x4::identityMat4x4, 
                    pAnimation.get(),
                    pSkeleton.get());

  for(SizeT i = 0; i < pSkeleton->bones.size(); ++i) {
    m_transforms[i] = pSkeleton->bones[i]->finalTransform;
  }
}

void 
Animator::readNodeHeirarchy(float animTime, 
                            Skeleton::NodeData* pNode, 
                            const Matrix4x4 & parentTransform,
                            const Animation* pAnimation,
                            const Skeleton* pSkeleton) {

  Matrix4x4 nodeTransform = pNode->transform;

  auto pBoneAnim = pAnimation->getBoneAnimation(pNode->name);

  if(pBoneAnim) {
    
    nodeTransform.identity();

    nodeTransform.Translation(interpolateTransform(pBoneAnim->positions,
                                                   animTime, 
                                                   0));
    
    Matrix4x4 rotMatrix;

    interpolateTransform(pBoneAnim->rotations,
                         animTime, 
                         0).matrixFromQuaternion(rotMatrix);

    nodeTransform *= rotMatrix;

    nodeTransform.Scale(interpolateTransform(pBoneAnim->scales,
                                             animTime, 
                                             0));    
  }

  Matrix4x4 globalTransform = parentTransform * nodeTransform;

  auto boneIt = pSkeleton->bonesMapping.find(pNode->name);

  if(boneIt != pSkeleton->bonesMapping.end()) {
    auto& pBone = pSkeleton->bones[boneIt->second];
    pBone->finalTransform = pSkeleton->gloabalInverseTransform * 
                            globalTransform *
                            pBone->boneOffset;
  }

  for(auto& pChild : pNode->children) {
    readNodeHeirarchy(animTime, 
                      pChild.get(), 
                      globalTransform, 
                      pAnimation,
                      pSkeleton);
  }

}

}


