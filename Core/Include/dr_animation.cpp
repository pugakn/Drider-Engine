#include "dr_animation.h"
#include <dr_math.h>

namespace driderSDK {

void 
Animation::setBoneAnimation(const TString& bone, 
                            BoneAnim&& boneAnimation) {
  SizeT newIndex = m_bonesAnimations.size();
  m_bonesAnimations[bone] = std::move(boneAnimation);

  DR_ASSERT(newIndex != m_bonesAnimations.size());

  m_bonesIndices[bone] = static_cast<UInt32>(newIndex);
}

const Animation::BoneAnim*
Animation::getBoneAnimation(const TString& bone) const {
  auto boneIt  = m_bonesAnimations.find(bone);

  const BoneAnim* pBone = nullptr;

  if (boneIt != m_bonesAnimations.end()) {
    pBone = &boneIt->second;
  }

  return pBone;
}

const UInt32 Animation::getBoneIndex(const TString& bone) const {
  auto boneIt = m_bonesIndices.find(bone);

  DR_ASSERT(boneIt != m_bonesIndices.end());

  return boneIt->second;
}


const std::unordered_map<TString, Animation::BoneAnim>& 
Animation::getBonesAnimations() {
  return m_bonesAnimations;
}


void 
Animation::setTicksPerSecond(float tps) {
  if (tps <= 0) {
    tps = 10.f;
  }
  
  m_ticksPerSecond = tps;
}

void 
Animation::setDuration(float durationInTicks) {
  m_duration = durationInTicks;
}

float 
Animation::getDurationInSecs() const {

  DR_ASSERT(m_ticksPerSecond);

  return m_duration / m_ticksPerSecond;
}

float Animation::getDurationInTicks() const {
  return m_duration;
}

float 
Animation::getTicksPerSecond() const {
  return m_ticksPerSecond;
}

}