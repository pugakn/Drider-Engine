#pragma once

namespace driderSDK {

namespace math {

class Vector3D
{
 public:
  Vector3D();
  Vector3D(Vector3D && V) = default;
  Vector3D(const Vector3D & V);
  Vector3D(float _x, float _y, float _z);
  virtual ~Vector3D();

  union
  {
    struct
    {
      float x, y, z;
    };
    float v[3];
  };
};

}

}