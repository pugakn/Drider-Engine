#pragma once

#include <unordered_map>
#include <dr_memory.h>
#include <dr_quaternion.h>
#include <dr_vector3d.h>
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

class Skeleton;

class Animation : public Resource {

public:
  template<class T>
  struct AnimationTransform
  {
    T value;
    float time;
  };

  template<class T>
  using AnimTransformations = std::vector<AnimationTransform<T>>;

  struct BoneAnim {
    AnimTransformations<Vector3D> positions;
    AnimTransformations<Quaternion> rotations;
    AnimTransformations<Vector3D> scales;
  };

  void 
  setBoneAnimation(const TString& bone, 
                   BoneAnim&& boneAnimation);

  const BoneAnim*
  getBoneAnimation(const TString& bone) const;

  const std::unordered_map<TString, BoneAnim>&
  getBonesAnimations();

  void
  setTicksPerSecond(float tps);

  void
  setDuration(float durationInTicks);

  float 
  getDurationInSecs();

  float
  getDurationInTicks();

  float
  getTicksPerSecond();

  void init(void*)
  {}

 private:

  float m_ticksPerSecond;
  float m_duration; 
  std::unordered_map<TString, BoneAnim> m_bonesAnimations;
};

}
