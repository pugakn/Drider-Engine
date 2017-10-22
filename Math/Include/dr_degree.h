#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {

class DR_API_EXPORT Degree
{
 public:
  /**
  * TEST::defaultConstructor
  * Default constructor.
  *
  */
  Degree();

  /**
  * TEST::moveConstructor
  * Move constructor.
  *
  */
  Degree(Degree&& V) = default;

  /**
  * TEST::copyConstructor
  * Copy constructor.
  *
  */
  Degree(const Degree& V);

  /**
  * Initialize class with value.
  *
  * @param value
  *  Initial value of the class.
  */
  explicit Degree(float value);

  /**
  * TEST::destructor
  * Default destructor.
  *
  */
  ~Degree();

  /**
  * TEST::ToRadian
  * Returns a Radian class with a value equal to the
  * actual degrees in radians.
  *
  * @return
  *   Class radian.
  */
  float
  toRadian() const;

  /**
  * TEST::Unwind
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
  * Sets the value to the float param.
  *
  * Param V
  *   The new value.
  */
  Degree&
  operator=(float V);

  //Uncomented
  Degree&
  operator+=(float V);

  //Uncomented
  Degree&
  operator-=(float V);

  //Uncomented
  Degree&
  operator*=(float V);

  //Uncomented
  Degree&
  operator/=(float V);
private:
  float m_value;
};

}