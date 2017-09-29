#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {

class DR_API_EXPORT Degree
{
 public:
  /**
  * Default constructor.
  *
  */
  Degree();

  /**
  * Move constructor.
  *
  */
  Degree(Degree&& V) = default;

  /**
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
  * Default destructor.
  *
  */
  ~Degree();

  /**
  * Returns a Radian class with a value equal to the
  * actual degrees in radians.
  *
  * @return
  *   Class radian.
  */
  float toRadian() const;

  /**
  * Limit the value in [0, 360)
  *
  * @return
  *   A reference to this class.
  */
  Degree&
  toRange();

  //Uncomented
  operator float();

  //Uncomented
  Degree&
  operator=(float V);
private:
  float m_value;
};

}