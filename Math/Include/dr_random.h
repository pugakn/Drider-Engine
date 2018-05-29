#pragma once
#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"
#include <random>
namespace driderSDK {
  class Random {
  public:
    static float RandomRange(const float min_, const float max_) {
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_real_distribution<float> uni(min_, max_);
      return uni(rng);
    }
    static Vector3D RandomRange(const Vector3D& min_, const Vector3D& max_) { 
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_real_distribution<float> uniX(min_.x, max_.x);
      std::uniform_real_distribution<float> uniY(min_.y, max_.y);
      std::uniform_real_distribution<float> uniZ(min_.z, max_.z);
      return Vector3D(uniX(rng), uniY(rng), uniZ(rng));
    }
  };
}