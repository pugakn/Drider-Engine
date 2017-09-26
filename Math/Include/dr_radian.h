#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {

class Degree;

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
  * Initialize class with value.
  *
  * @param value
  *  Initial value.
  */
  Radian(const Degree& V);

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
  Degree toDegree() const;

  //Uncomented
  operator float();

  //Uncomented
  Radian&
  operator=(float V);

  //Uncomented
  Radian&
  operator=(const Degree& V);
private:
  friend class Degree;
  float m_value;
};

}