#pragma once

#include <cmath>
#include <dr_prerequisites.h>
#include "dr_matrix4x4.h"
#include "dr_matrix3x3.h"

namespace driderSDK {

class DR_API_EXPORT Quaternion
{
 public:

  /**
  * Default constructor.
  *
  */
  Quaternion();

  /**
  * Move constructor.
  *
  */
  Quaternion(Quaternion&& Q) = default;

  /**
  * Copy constructor.
  *
  */
  Quaternion(const Quaternion& Q);

  /**
  * Initialize the constructor with the given values.
  *
  * @param x
  *   The x value of the quaternion.
  *
  * @param y
  *   The y value of the quaternion.
  *
  * @param z
  *   The z value of the quaternion.
  *
  * @param w
  *   The w value of the quaternion.
  */
  Quaternion(float x, float y, float z, float w);

  /**
  * Default destructor.
  *
  */
  ~Quaternion();

  /**
  * Computes the measure of the quaternion.
  *
  * @return
  *	  Quaternion's lenght.
  */
  float
  measure();

  /**
  * Computes the conjugate of the quaternion.
  *
  * @return
  *   The conjugate quaternion.
  */
  Quaternion
  conjugate();

  /**
  * Computes the normalized quaternion.
  *
  * @return
  *   This normalized quaternion.
  */
  Quaternion
  normalize();

  /**
  * Rotates the quaternion given theta and another quaternion.
  *
  * @param theta
  *	  Angle of rotation.
  *
  * @param A
  *   The other quaternion to generate an axis.
  *
  * @return
  *   A rotated quaternion.
  */
  Quaternion
  rotation(float theta, const Quaternion& A);

  /**
  * Creates a 4D matrix from the quaternion.
  *
  * @param Matrix
  *   Matrix4x4 to be filled.
  */
  void
  matrixFromQuaternion(Matrix4x4& Matrix);

	/**
	* Creates a 3D matrix from the quaternion.
	*
	* @param Matrix
	*   Matrix3x3 to be filled.
	*/
	void
	matrixFromQuaternion(Matrix3x3& Matrix);

  //Uncommented
  Quaternion
  operator+(const Quaternion& Q) const;

  //Uncommented
  Quaternion&
  operator+=(const Quaternion& Q);

  //Uncommented
  Quaternion
  operator-(const Quaternion& Q) const;

  //Uncommented
  Quaternion&
  operator-=(const Quaternion& Q);

  //Uncommented
  Quaternion
  operator*(const Quaternion& Q) const;

  //Uncommented
  Quaternion
  operator*(float s) const;

  //Uncommented
  Quaternion&
  operator*=(const Quaternion& Q);

  //Uncommented
  Quaternion&
  operator*=(float s);

  //Uncommented
  Quaternion
  operator/(const Quaternion& Q) const;

  //Uncommented
  Quaternion&
  operator/=(const Quaternion& Q);

  union 
  {
    struct 
    {
	  float x, y, z, w;
    };
	float data[4];
  };

};

}