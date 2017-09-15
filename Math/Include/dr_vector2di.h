#pragma once

namespace driderSDK {

namespace math {

class Vector2DI
{
 public:
  Vector2DI();
  Vector2DI(Vector2DI && V) = default;
  Vector2DI(const Vector2DI & V);
  Vector2DI(__int32 _x, __int32 _y);
  virtual ~Vector2DI();

  union
  {
    struct
    {
      __int32 x, y;
    };
    __int32 v[2];
  };
};

}

}