#pragma once

#include <cmath>
#include <dr_prerequisites.h>
#include "dr_matrix4x4.h"
#include "dr_matrix3x3.h"
#include "dr_vector4d.h"

namespace driderSDK {

/**
*  Quaternion class with real and imaginary values.
*
* Sample usage:
*	Quaternion myQuaternion(0.5, 0.1, 0.0, 1.0);
*/
class DR_API_EXPORT Quaternion
{
 public:

  /**
	* TEST::defaultConstructor
  * Default constructor.
  *
  */
  Quaternion();

	/**
	* TEST::forceInitZero
	* Default constructor
	*
	* @param k
	*  Values are initialized with 0(kZero) or identity quaternion(kIdentity)
	*/
	explicit Quaternion(Math::FORCE_INIT k);

  /**
	* TEST::moveConstructor
  * Move constructor.
  *
  */
  Quaternion(Quaternion&& Q) = default;

  /**
	* TEST::constructFromQuaternion
  * Copy constructor.
  *
  */
  Quaternion(const Quaternion& Q);

  /**
	* TEST::constructFrom4floats
  * Initialize the constructor with the given values.
  *
  * @param _x
  *   The x value of the quaternion.
  *
  * @param _y
  *   The y value of the quaternion.
  *
  * @param _z
  *   The z value of the quaternion.
  *
  * @param _w
  *   The w value of the quaternion.
  */
  Quaternion(float _x, float _y, float _z, float _w);

	/**
	* TEST::constructFromVector4D
	* Initialize the constructor with a vector4D
	*
	* @param V
	*   The vector to construct the quaternion
	*/
	Quaternion(const Vector4D& V);

  /**
	* TEST::destructor
  * Default destructor.
  *
  */
  ~Quaternion();

  /**
	* TEST::measure
  * Computes the measure of the quaternion.
  *
  * @return
  *	  Quaternion's lenght.
  */
  float
	measure();

  /**
	* TEST::conjugate
  * Computes the conjugate of the quaternion.
  *
  * @return
  *   The conjugate quaternion.
  */
  Quaternion
  conjugate();

  /**
	* TEST::normalize
  * Normalize the quaternion.
  *
  */
  void
  normalize();

  /**
	* TEST::rotation
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
	* TEST::matrixFromQuaternion4x4
  * Creates a 4D matrix from the quaternion.
  *
  * @param MatrixOut
  *   Matrix4x4 to be filled.
  */
  void
  matrixFromQuaternion(Matrix4x4& MatrixOut);

	/**
	* TEST::matrixFromQuaternion3x3
	* Creates a 3D matrix from the quaternion.
	*
	* @param MatrixOut
	*   Matrix3x3 to be filled.
	*/
	void
	matrixFromQuaternion(Matrix3x3& MatrixOut);

	/**
	* Gets a pointer to the first element of the quaternion.
	*
	* @return
	*   A pointer to the first element.
	*/
	float*
	ptr();

	/**
	* Gets a constant pointer to the first element of the quaternion.
	*
	* @return
	*   A constant pointer to the first element.
	*/
	const float*
	ptr() const;

	/**
	* TEST::operatorBrackets
	* Gets a reference to a specific element of the quaternion.
	*
	* @return
	*   Reference to an element of the quaternion.
	*/
	float&
	operator[](const SizeT index);

	/**
	* Gets a constant reference to a specific element of the quaternion.
	*
	* @return
	*   Constant reference to an element of the quaternion.
	*/
	const float&
	operator[](const SizeT index) const;

	/**
	* TEST::operatorAdd
	* Adds two quaternions.
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   The result quaternion of the operation.
	*/
  Quaternion
  operator+(const Quaternion& Q) const;

	/**
	* TEST::operatorAddEquals
	* Adds two quaternions and stores the result in this quaternion
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   A reference to this quaternion
	*/
  Quaternion&
  operator+=(const Quaternion& Q);

	/**
	* TEST::operatorMinus
	* Substracts two quaternions.
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   The result quaternion of the operation.
	*/
  Quaternion
  operator-(const Quaternion& Q) const;

	/**
	* TEST::operatorMinusEquals
	* Substracts two quaternions and stores the result in this quaternion
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   A reference to this quaternion
	*/
  Quaternion&
  operator-=(const Quaternion& Q);

	/**
	* TEST::operatorMultiplyByQuaternion
	* Multiplies two quaternions.
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   The result quaternion of the operation.
	*/
  Quaternion
  operator*(const Quaternion& Q) const;

	/**
	* TEST::operatorMultiplyByScalar
	* Multiplies quaternion with an scalar.
	*
	* @param s
	*  The value to operate with.
	*
	* @return
	*   The result quaternion of the operation.
	*/
  Quaternion
  operator*(float s) const;

	/**
	* TEST::operatorMultiplyByQuaternionEquals
	* Multiplies two quaternions and stores the result in this quaternion
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   A reference to this quaternion
	*/
  Quaternion&
  operator*=(const Quaternion& Q);

	/**
	* TEST::operatorMultiplyByScalarEquals
	* Multiplies quaternion with an scalar and stores the result in this quaternion
	*
	* @param s
	*  The value to operate with.
	*
	* @return
	*   A reference to this quaternion
	*/
  Quaternion&
  operator*=(float s);

	/**
	* TEST::operatorDivide
	* Divides two quaternions.
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   The result quaternion of the operation.
	*/
  Quaternion
  operator/(const Quaternion& Q) const;

	/**
	* TEST::operatorDivideEquals
	* Divides two quaternions and stores the result in this quaternion
	*
	* @param Q
	*  The other quaternion.
	*
	* @return
	*   A reference to this quaternion
	*/
  Quaternion&
  operator/=(const Quaternion& Q);

	/**
	* TEST::operatorAssign
	* Assigns the values from another quaternion
	*
	* @param Q
	*  The quaternion to take the values from.
	*
	* @return
	*   A reference to this quaternion
	*/
	Quaternion&
	operator=(const Quaternion& Q);

	/**
	* TEST::operatorEqual
	* Checks if quaternion is equal to another quaternion
	*
	* @param Q
	*  The quaternion to compare.
	*
	* @return
	*   True if it is equal
	*/
	bool
	operator==(const Quaternion& Q);

	/**
	* TEST::operatorNotEqual
	* Checks if quaternion is not equal to another quaternion
	*
	* @param Q
	*  The quaternion to compare.
	*
	* @return
	*   True if it is not equal
	*/
	bool
	operator!=(const Quaternion& Q);

	/**
	* TEST::operatorNot
	* Returns this quaternion with negated values.
	*
	*/
	Quaternion
	operator-() const;

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