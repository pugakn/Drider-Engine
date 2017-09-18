#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Vector2DI
{
 public:
  /**
  * Default constructor
  *
  * Values are initialized with 0.
  */

  Vector2DI();

  /**
  * Move constructor
  *
  */

  Vector2DI(Vector2DI&& V) = default;

  /**
  * Copy constructor
  *
  */

  Vector2DI(const Vector2DI& V);

  /**
  * Initialize constructor with values.
  *
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  */

  Vector2DI(Int32 x, Int32 y);

  /**
  * Default destructor
  *
  */

  ~Vector2DI();

  /**
  * Computes the dot product between this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  The vector against which the dot product is calculated.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */

  Float32
  dot(const Vector2DI& B) const;

  /**
  * Computes the magnitude of this vector.
  *
  * @return
  *   The magnitude (or "size") of the vector.
  */

  Float32
  magnitude() const;

  /**
  * Get this vector normalized.
  *
  * @return
  *   This vector normalized.
  */

  Vector2DI
  normalize() const;

  //Uncommented

  Vector2DI
  operator+(const Vector2DI & A) const;

  //Uncommented

  Vector2DI&
  operator+=(const Vector2DI & A);

  //Uncommented

  Vector2DI
  operator-(const Vector2DI & A) const;

  //Uncommented

  Vector2DI&
  operator-=(const Vector2DI & A);

  //Uncommented

  Vector2DI
  operator*(const Vector2DI & A) const;

  //Uncommented

  Vector2DI&
  operator*=(const Vector2DI & A);

  //Uncommented

  Vector2DI
  operator*(const Float32 S) const;

  //Uncommented

  Vector2DI&
  operator*=(const Float32 S);

  //Uncommented

  Vector2DI
  operator/(const Float32 S) const;

  //Uncommented

  Vector2DI&
  operator/=(const Float32 S);

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