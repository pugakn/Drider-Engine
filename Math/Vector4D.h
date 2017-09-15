#pragma once

namespace driderSDK {

namespace math {

class Vector4D
{
 public:
  Vector4D();
  Vector4D(Vector4D && V) = default;
  Vector4D(const Vector4D & V);
  Vector4D(float _x, float _y, float _z, float _w);
  virtual ~Vector4D();

  union
  {
    struct
    {
      float x, y, z, w;
    };
    float v[4];
  };
};

}

}