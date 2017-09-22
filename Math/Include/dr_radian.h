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
  Radian(Float32 value);

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
  operator Float32();

  //Uncomented
  Radian&
  operator=(Float32 V);

  //Uncomented
  Radian&
  operator=(const Degree& V);
private:
  friend class Degree;
  Float32 m_value;
};

}