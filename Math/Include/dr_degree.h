#pragma once

#include <dr_prerequisites.h>
#include <dr_math.h>

namespace driderSDK {

class Radian;

class Degree
{
 public:
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
  Degree(Float32 value);

  /**
  * Initialize with Radian.
  *
  * @param value
  *  Initial value of the class.
  */
  Degree(const Radian& value);

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
  Radian toRadian() const;

  //Uncomented
  operator Float32();

  //Uncomented
  Degree&
  operator=(Float32 V);

  //Uncomented
  Degree&
  operator=(const Radian& V);
private:
  friend class Radian;
  Float32 m_value;
};

}