#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

namespace math {

class DR_API_EXPORT Quaternion
{
 public:
  /**
  * Default constructor.
  *
  * Initialize the values on (0,0,0,1).
  */

  Quaternion();

  /**
  * Move constructor.
  *
  */

  Quaternion(Quaternion && Q) = default;

  /**
  * Copy constructor.
  *
  */

  Quaternion(const Quaternion & Q);

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

  Quaternion(Float32 x, Float32 y, Float32 z, Float32 w);

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

  Float32
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
  * @param t
  *	  Angle of rotation.
  *
  * @param A
  *   The other quaternion to generate an axis.
  *
  * @return
  *   A rotated quaternion.
  */

  Quaternion
  rotation(Float32 t, const Quaternion & A);

  /**
  * Creates a matrix from the quaternion.
  *
  * @return
  *   ...
  */

  void
  matrixFromQuaternion();

  //Uncommented

  Quaternion
  operator+(const Quaternion & Q) const;

  //Uncommented

  Quaternion&
  operator+=(const Quaternion & Q);

  //Uncommented

  Quaternion
  operator-(const Quaternion & Q) const;

  //Uncommented

  Quaternion&
  operator-=(const Quaternion & Q);

  //Uncommented

  Quaternion
  operator*(const Quaternion & Q) const;

  //Uncommented

  Quaternion
  operator*(Float32 s) const;

  //Uncommented

  Quaternion&
  operator*=(const Quaternion & Q);

  //Uncommented

  Quaternion&
  operator*=(Float32 s);

  //Uncommented

  Quaternion
  operator/(const Quaternion & Q) const;

  //Uncommented

  Quaternion&
  operator/=(const Quaternion & Q);

  Float32 x, y, z, w;

};

}

}