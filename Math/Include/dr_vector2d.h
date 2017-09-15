#pragma once

/*
Delete this comment:

Declaration Order:
  Typedefs and enums
  Constructors
  Destructors
  Methods, including static methods
  Constants (static const data members)
  Data members (Except static const data members)
*/

namespace driderSDK {

namespace math {

class Vector2D
{
 public:
  Vector2D();
  Vector2D(Vector2D && V) = default;
  Vector2D(const Vector2D & V);
  Vector2D(float _x, float _y);
  virtual ~Vector2D();

  union
  {
    struct
    {
      float x, y;
    };
    float v[2];
  };
};

}

}