#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Vector2D
{
 public:
  /**
  * Default constructor
  *
  * Values are initialized with 0.
  */

  Vector2D()
	: x(0.0f),
	  y(0.0f)
  {}

  /**
  * Move constructor
  *
  */

  Vector2D(Vector2D && V) = default;

  /**
  * Copy constructor
  *
  */

  Vector2D(const Vector2D & V)
    : x(V.x),
      y(V.y)
  {}

  /**
  * Initialize constructor with values.
  *
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  */

  Vector2D(Float32 x, Float32 y)
    : x(x),
      y(y)
  {}

  /**
  * Default destructor
  *
  */

  ~Vector2D()
  {}

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

  FORCEINLINE Float32
  dot(const Vector2D B) const
  {
  return (x*B.x) + (y*B.y);
  };

  /**
  * Computes the magnitude of this vector.
  *
  * @return
  *   The magnitude (or "size") of the vector.
  */

  FORCEINLINE Float32
  magnitude() const
  {
  return sqrt(dot(*this));
  };

  /**
  * Get this vector normalized.
  *
  * @return
  *   This vector normalized.
  */

  FORCEINLINE Vector2D
  normalize() const
  {
  return (*this) * pow(magnitude(), -1);
  };

  //Uncommented

  FORCEINLINE Vector2D
  operator+ (const Vector2D & A) const
  {
  return Vector2D(x + A.x, y + A.y);
  };

  //Uncommented

  FORCEINLINE Vector2D&
  operator+= (const Vector2D & A)
  {
  x += A.x;
  y += A.y;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector2D
  operator- (const Vector2D & A) const
  {
  return Vector2D(x - A.x, y - A.y);
  };

  //Uncommented

  FORCEINLINE Vector2D&
  operator-= (const Vector2D & A)
  {
  x -= A.x;
  y -= A.y;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector2D
  operator* (const Vector2D & A) const
  {
  return Vector2D(x*A.x, y*A.y);
  };

  //Uncommented

  FORCEINLINE Vector2D&
  operator*= (const Vector2D & A)
  {
  x *= A.x;
  y *= A.y;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector2D
  operator* (const Float32 S) const
  {
    return Vector2D(x*S, y*S);
  };

  //Uncommented

  FORCEINLINE Vector2D&
  operator*= (const Float32 S)
  {
    x *= S;
    y *= S;
    return *this;
  };

  //Uncommented

  FORCEINLINE Vector2D
  operator/ (const Float32 S) const
  {
	return Vector2D(x*pow(S, -1), y*pow(S, -1));
  };

  //Uncommented

  FORCEINLINE Vector2D&
  operator/= (const Float32 S)
  {

	x *= pow(S, -1);
	y *= pow(S, -1);
	return *this;
  };

  union
  {
    struct
    {
      Float32 x, y;
    };
    Float32 v[2];
  };
};

}