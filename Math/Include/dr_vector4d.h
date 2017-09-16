#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Vector4D
{
 public:
  /**
  * Default constructor
  *
  * Values are initialized with 0.
  */

  Vector4D()
    : x(0.0f),
      y(0.0f),
      z(0.0f),
      w(0.0f)
  {};

  /**
  * Move constructor
  *
  */

  Vector4D(Vector4D && V) = default;

  /**
  * Copy constructor
  *
  */

  Vector4D(const Vector4D & V)
    : x(V.x),
      y(V.y),
      z(V.z),
      w(V.w)
  {};

  /**
  * Initialize constructor with values.
  *
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  *
  * @param z
  *  The z value of the vector
  *
  * @param w
  *  The w value of the vector
  */

  Vector4D(Float32 x, Float32 y, Float32 z, Float32 w)
    : x(x),
      y(y),
      z(z),
      w(w)
  {};

  /**
  * Default destructor
  *
  */

  ~Vector4D()
  {};

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
  dot(const Vector4D B) const
  {
  return (x*B.x) + (y*B.y) + (z*B.z) + (w*B.w);
  };

  /**
  * Computes the cross product between this vector and the vector parameter.
  * W value is not used, and it's final value will be 0.
  * This operatios is NOT commutative.
  *
  * @param B
  *  The vector against which the cross product is calculated.
  *  B (vector parameter) is the rigth value of operation
  *  AxB
  *
  * @return
  *   Result vector of the cross product
  */

  FORCEINLINE Vector4D
  cross(const Vector4D B) const
  {
  return Vector4D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x, 0.0f);
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

  FORCEINLINE Vector4D
  normalize() const
  {
  return (*this) * pow(magnitude(), -1);
  };

  //Uncommented

  FORCEINLINE Vector4D
  operator+ (const Vector4D & A) const
  {
  return Vector4D(x + A.x, y + A.y, z + A.z, w + A.w);
  };

  //Uncommented

  FORCEINLINE Vector4D&
  operator+= (const Vector4D & A)
  {
  x += A.x;
  y += A.y;
  z += A.z;
  w += A.w;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector4D
  operator- (const Vector4D & A) const
  {
  return Vector4D(x - A.x, y - A.y, z - A.z, w - A.w);
  };

  //Uncommented

  FORCEINLINE Vector4D&
  operator-= (const Vector4D & A)
  {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  w -= A.w;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector4D
  operator* (const Vector4D & A) const
  {
  return Vector4D(x*A.x, y*A.y, z*A.z, w * A.w);
  };

  //Uncommented

  FORCEINLINE Vector4D&
  operator*= (const Vector4D & A)
  {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  w *= A.w;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector4D
  operator* (const Float32 S) const
  {
    return Vector4D(x*S, y*S, z*S, w*S);
  };

  //Uncommented

  FORCEINLINE Vector4D&
  operator*= (const Float32 S)
  {
    x *= S;
    y *= S;
    z *= S;
    w *= S;
    return *this;
  };

  //Uncommented

  FORCEINLINE Vector4D
  operator/ (const Float32 S) const
  {
  return Vector4D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1), w*pow(S, -1));
  };

  //Uncommented

  FORCEINLINE Vector4D&
  operator/= (const Float32 S)
  {
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  w *= pow(S, -1);
  return *this;
  };

  union
  {
    struct
    {
      Float32 x, y, z, w;
    };
    Float32 v[4];
  };
};

}