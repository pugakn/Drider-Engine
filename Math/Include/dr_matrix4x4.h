#pragma once

#include "dr_vector4d.h"
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Matrix4x4
{
 public:
  enum INIT_MATRIX {
    kZero,
    kIdentity
  };

  /**
  * Default constructor
  *
  */
  Matrix4x4();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  Matrix4x4(INIT_MATRIX k);

  /**
  * Move constructor
  *
  */
  Matrix4x4(Matrix4x4&& M) = default;

  /**
  * Copy constructor
  *
  */
  Matrix4x4(const Matrix4x4& M);

  /**
  * Initialize constructor with values.
  *
  * @param v0x, v0y, v0z, v0w
  *  The x, y, z, w values of the vector, first raw
  *
  * @param v1x, v1y, v1z, v1w
  *  The x, y, z, w values of the vector, second raw
  *
  * @param v2x, v2y, v2z, v2w
  *  The x, y, z, w values of the vector, third raw
  *
  * @param v3x, v3y, v3z, v3w
  *  The x, y, z, w values of the vector, third raw
  *
  */
  Matrix4x4(Float32 v0x, Float32 v0y, Float32 v0z, Float32 v0w,
            Float32 v1x, Float32 v1y, Float32 v1z, Float32 v1w,
            Float32 v2x, Float32 v2y, Float32 v2z, Float32 v2w,
            Float32 v3x, Float32 v3y, Float32 v3z, Float32 v3w);

  /**
  * Initialize constructor with values.
  *
  * @param v0
  *  The vector0(x, y, z, w) values of the vector, first raw
  *
  * @param v1
  *  The vector1(x, y, z, w) values of the vector, second raw
  *
  * @param v3
  *  The vector2(x, y, z, w) values of the vector, third raw
  *
  * @param v4
  *  The vector2(x, y, z, w) values of the vector, quarter raw
  */
  Matrix4x4(Vector4D vector0, Vector4D vector1, Vector4D vector2, Vector4D vector3);

  /**
  * Default destructor
  *
  */
  ~Matrix4x4();

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
  Matrix4x4
    inverse() const;

  //Uncommented
  Vector4D&
    operator[](SizeT index);

  //Uncommented
  Matrix4x4&
    operator=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
    operator+(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
    operator+=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
    operator-(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
    operator-=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
    operator*(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
    operator*=(const Matrix4x4& A);

  //Uncommented
  Vector4D
    operator*(const Vector4D S) const;

  //Uncommented
  Matrix4x4
    operator*(const Float32 S) const;

  //Uncommented
  Matrix4x4&
    operator*=(const Float32 S);

  Vector4D v0, v1, v2, v3;
 protected:
 private:
};

}