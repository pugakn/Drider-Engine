#pragma once

#include "dr_math_prerequisites.h"
#include <dr_math.h>

namespace driderSDK {
/**
*  Radian class with value representing the angle.
*
* Sample usage:
*	Radian RaidanName;
*/
class DR_MATH_EXPORT Radian
{
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  Radian();

  /**
  * TEST::moveConstructor
  *
  * Move constructor.
  *
  */
  Radian(Radian&& V) = default;

  /**
  * TEST::copyConstructor
  *
  * Copy constructor.
  */
  Radian(const Radian& V);

  /**
  * TEST::floatConstructor
  *
  * Initialize class with value.
  *
  * @param value
  *  Initial value.
  */
  explicit Radian(float value);

  /**
  * TEST::defaultDestructor
  *
  * Default destructor.
  */
  ~Radian();

  /**
  * TEST::ToDegree
  *
  * Returns a Degree value equal to the
  * actual radian in degrees.
  *
  * @return
  *   Class degree.
  */
  float
  toDegree() const;

  /**
  * TEST::Unwind
  *
  * Limit the value in [0, 360)
  *
  * @return
  *   A reference to this class.
  */
  Radian&
  unwind();

  /**
  * TEST::floatOperator
  *
  * @eeturn
  *   The class value as a float.
  */
  operator float();

  /**
  * TEST::equalFloat
  *
  * Sets the class value to the given float.
  *
  * @return
  *   Reference to this.
  */
  Radian&
  operator=(float V);

  /**
  * TEST::plusEqualFloat
  *
  * Add the given float to the class value.
  *
  * @return
  *   Reference to this.
  */
  Radian&
  operator+=(float V);

  /**
  * TEST::lessEqualFloat
  *
  * Subs the given float to the class value.
  *
  * @return
  *   Reference to this.
  */
  Radian&
  operator-=(float V);

  /**
  * TEST::mulEqualFloat
  *
  * Multiplies the given float to the class value.
  *
  * @return
  *   Reference to this.
  */
  Radian&
  operator*=(float V);


  /**
  * TEST::divEqualFloat
  *
  * Divides the class value with the given float.
  *
  * @return
  *   Reference to this.
  */
  Radian&
  operator/=(float V);
private:
  float m_value;
};

}