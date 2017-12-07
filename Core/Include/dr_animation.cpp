#include "dr_animation.h"

namespace driderSDK {

void 
Animation::setBoneAnimation(const TString& bone, 
                            BoneAnim&& boneAnimation) {
  m_bonesAnimations[bone] = std::move(boneAnimation);
}

const Animation::BoneAnim*
Animation::getBoneAnimation(const TString& bone) const {

  const BoneAnim* pBoneAnim = nullptr;

 auto boneIt  = m_bonesAnimations.find(bone);

  if (boneIt != m_bonesAnimations.end()) {
    pBoneAnim = &boneIt->second;
  }

  return pBoneAnim;
}

const std::unordered_map<TString, Animation::BoneAnim>& 
Animation::getBonesAnimations() {
  return m_bonesAnimations;
}


void 
Animation::setTicksPerSecond(float tps) {
  if(tps <= 0) {
    tps = 10.f;
  }
  
  m_ticksPerSecond = tps;
}

void 
Animation::setDuration(float durationInTicks) {
  m_duration = m_duration;
}

float 
Animation::getDurationInSecs() {

  DR_ASSERT(m_ticksPerSecond);

  return m_duration / m_ticksPerSecond;
}

float Animation::getDurationInTicks() {
  return m_duration;
}

float 
Animation::getTicksPerSecond() {
  return m_ticksPerSecond;
}

}