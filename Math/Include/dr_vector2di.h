#pragma once

/*
Remove this comment:
.h Include order:
1. C library
2. C++ Library
3. Other libraries .h or .hpp
4. Our project .h or .hpp
*/

#include <cstring>              //C Library
#include <dr_prerequisites.h>   //Our project .h? or other library .h?

namespace driderSDK {

namespace math {

class Vector2DI
{
 public:
  Vector2DI();
  Vector2DI(Vector2DI && V) = default;
  Vector2DI(const Vector2DI & V);
  Vector2DI(Int32 _x, Int32 _y);
  virtual ~Vector2DI();

  union
  {
    struct
    {
      Int32 x, y;
    };
    Int32 v[2];
  };
};

}

}