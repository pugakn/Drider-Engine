#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {
/**
*  Degree class with value representing the angle.
*
* Sample usage:
*	Degree RaidanName;
*/
class DR_API_EXPORT Degree
{
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  Degree();

  /**
  * TEST::moveConstructor
  *
  * Move constructor.
  */
  Degree(Degree&& V) = default;

  /**
  * TEST::copyConstructor
  *
  * Copy constructor.
  */
  Degree(const Degree& V);

  /**
  * TEST::floatConstructor
  *
  * Initialize class with value.
  *
  * @param value
  *  Initial value of the class.
  */
  explicit Degree(float value);

  /**
  * TEST::defaultDestructor
  *
  * Default destructor.
  */
  ~Degree();

  /**
  * TEST::toRadian
  *
  * Returns a Radian class with a value equal to the
  * actual degrees in radians.
  *
  * @return
  *   Class radian.
  */
  float
  toRadian() const;

  /**
  * TEST::unwind
  *
  * Limit the value in [0, 360)
  *
  * @return
  *   A reference to this class.
  */
  Degree&
  unwind();

  /**
  * TEST::floatOperator
  * return the value as a float.
  *
  * @return
  *   A float value.
  */
  operator float();

  /**
  * TEST::equalFloat
  *
  * Sets the value to the float param.
  *
  * @param V
  *   The new value.
  *
  * return
  *   A reference to this.
  */
  Degree&
  operator=(float V);

  /**
  * TEST::plusEqualFloat
  *
  * Adds the given float to the class value.
  *
  * @param V
  *   The new value.
  *
  * return
  *   A reference to this.
  */
  Degree&
  operator+=(float V);

  /**
  * TEST::lessEqualFloat
  *
  * Subs the given float to the class value.
  *
  * @param V
  *   The value to sub.
  *
  * return
  *   A reference to this.
  */
  Degree&
  operator-=(float V);

  /**
  * TEST::mulEqualFloat
  *
  * Multiplies the given float to the class value.
  *
  * @param V
  *   The factor value.
  *
  * return
  *   A reference to this.
  */
  Degree&
  operator*=(float V);

  /**
  * TEST::divEqualFloat
  *
  * Divides the class value with the given float.
  *
  * @param V
  *   The dividend value.
  *
  * return
  *   A reference to this.
  */
  Degree&
  operator/=(float V);
private:
  float m_value;
};

}