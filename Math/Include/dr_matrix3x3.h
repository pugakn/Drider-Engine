#pragma once

#include "dr_vector3d.h"
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Matrix3x3                                                                 
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
  Matrix3x3();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  Matrix3x3(INIT_MATRIX k);

  /**
  * Move constructor
  *
  */
  Matrix3x3(Matrix3x3&& M) = default;

  /**
  * Copy constructor
  *
  */
  Matrix3x3(const Matrix3x3& M);

  /**
  * Initialize constructor with values.
  *
  * @param v00-v02
  *  The x, y, z values of the vector, first raw
  *
  * @param v10-v12
  *  The x, y, z values of the vector, second raw
  *
  * @param v20-v22
  *  The x, y, z values of the vector, third raw
  */
  Matrix3x3(Float32 v1x, Float32 v1y, Float32 v1z,
            Float32 v2x, Float32 v2y, Float32 v2z,
            Float32 v3x, Float32 v3y, Float32 v3z);

  /**
  * Initialize constructor with values.
  *
  * @param v0
  *  The vector0(x, y, z) values of the vector, first raw
  *
  * @param v10-v12
  *  The vector1(x, y, z) values of the vector, second raw
  *
  * @param v20-v22
  *  The vector2(x, y, z) values of the vector, third raw
  */
  Matrix3x3(Vector3D vector0, Vector3D vector1, Vector3D vector2);

  /**
  * Default destructor
  *
  */
  ~Matrix3x3();

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
  Matrix3x3
  inverse() const;

   //Uncommented
  Vector3D&
  operator[](SizeT index);

  //Uncommented
  Matrix3x3&
  operator=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator+(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator+=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator-(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator-=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator*(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator*=(const Matrix3x3& A);

  //Uncommented
  Vector3D
  operator*(const Vector3D S) const;

  //Uncommented
  Matrix3x3
  operator*(const Float32 S) const;

  //Uncommented
  Matrix3x3&
  operator*=(const Float32 S);

  Vector3D v0, v1, v2;
 protected:
 private:
};

}