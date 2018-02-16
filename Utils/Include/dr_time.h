#pragma once

#include <chrono>
#include "dr_util_prerequisites.h"

namespace driderSDK {

using Seconds = float;
using Milliseconds = Int32;
using Microseconds = Int64;

class DR_UTIL_EXPORT Time 
{
 public:
  using Duration = std::chrono::microseconds;

  explicit Time(Seconds microseconds);
  explicit Time(Milliseconds milliseconds);
  explicit Time(Microseconds seconds);

  Time();

  float 
  asSeconds() const;

  Int32 
  asMilliseconds() const;

  Int64 
  asMicroseconds() const;

  void 
  fromSeconds(float seconds);

  void 
  fromMilliseconds(Int32 milliseconds);

  void 
  fromMicroseconds(Int64 microseconds);

  bool
  operator<(const Time& rhs);

  bool
  operator>(const Time& rhs);

  bool 
  operator>=(const Time& rhs);

  bool 
  operator<=(const Time& rhs);

  bool 
  operator==(const Time& rhs);

	bool
  operator!=(const Time& rhs);

	Time& 
  operator%=(const Time& rhs);

	Time& 
  operator+=(const Time& rhs);

	Time& 
  operator-=(const Time& rhs);

	Time  
  operator%(Time rhs);

	Time  
  operator+(Time rhs);

	Time  
  operator-(Time rhs);

 private:
  Duration m_duration;
};

}
