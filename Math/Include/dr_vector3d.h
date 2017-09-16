#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Vector3D
{
 public:
  /**
  * Default constructor
  *
  * Values are initialized with 0.
  */

  Vector3D()
    : x(0.0f),
      y(0.0f),
      z(0.0f)
  {};

  /**
  * Move constructor
  *
  */

  Vector3D(Vector3D && V) = default;

  /**
  * Copy constructor
  *
  */

  Vector3D(const Vector3D & V)
    : x(V.x),
      y(V.y),
      z(V.z)
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
  */

  Vector3D(Float32 x, Float32 y, Float32 z)
    : x(x),
      y(y),
      z(z)
  {};

  /**
  * Default destructor
  *
  */

  ~Vector3D()
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
  dot(const Vector3D B) const
  {
  return (x*B.x) + (y*B.y) + (z*B.z);
  };

  /**
  * Computes the cross product between this vector and the vector parameter.
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

  FORCEINLINE Vector3D
  cross(const Vector3D B) const
  {
  return Vector3D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
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

  FORCEINLINE Vector3D
  normalize() const
  {
  return (*this) * pow(magnitude(), -1);
  };

  //Uncommented

  FORCEINLINE Vector3D
  operator+ (const Vector3D & A) const
  {
  return Vector3D(x + A.x, y + A.y, z + A.z);
  };

  //Uncommented

  FORCEINLINE Vector3D&
  operator+= (const Vector3D & A)
  {
  x += A.x;
  y += A.y;
  z += A.z;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector3D
  operator- (const Vector3D & A) const
  {
  return Vector3D(x - A.x, y - A.y, z - A.z);
  };

  //Uncommented

  FORCEINLINE Vector3D&
  operator-= (const Vector3D & A)
  {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector3D
  operator* (const Vector3D & A) const
  {
  return Vector3D(x*A.x, y*A.y, z*A.z);
  };

  //Uncommented

  FORCEINLINE Vector3D&
  operator*= (const Vector3D & A)
  {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  return *this;
  };

  //Uncommented

  FORCEINLINE Vector3D
  operator* (const Float32 S) const
  {
    return Vector3D(x*S, y*S, z*S);
  };

  //Uncommented

  FORCEINLINE Vector3D&
  operator*= (const Float32 S)
  {
    x *= S;
    y *= S;
    z *= S;
    return *this;
  };

  //Uncommented

  FORCEINLINE Vector3D
  operator/ (const Float32 S) const
  {
  return Vector3D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1));
  };

  //Uncommented

  FORCEINLINE Vector3D&
  operator/= (const Float32 S)
  {
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  return *this;
  };

  union
  {
    struct
    {
      Float32 x, y, z;
    };
    Float32 v[3];
  };
};

}