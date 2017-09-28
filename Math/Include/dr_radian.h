#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {

class Radian
{
 public:
  /**
  * Default constructor.
  *
  */
  Radian();

  /**
  * Move constructor.
  *
  */
  Radian(Radian&& V) = default;

  /**
  * Copy constructor.
  *
  */
  Radian(const Radian& V);

  /**
  * Initialize class with value.
  *
  * @param value
  *  Initial value.
  */
  Radian(float value);

  /**
  * Default destructor.
  *
  */
  ~Radian();

  /**
  * Returns a Degree class with a value equal to the
  * actual radian in degrees.
  *
  * @return
  *   Class degree.
  */
  float toDegree() const;

  /**
  * Limit the value in [0, 360)
  *
  * @return
  *   A reference to this class.
  */
  Radian&
  toRange();

  //Uncomented
  operator float();

  //Uncomented
  Radian&
  operator=(float V);
private:
  float m_value;
};

}