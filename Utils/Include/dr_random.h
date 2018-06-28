#pragma once

#include <random>

#include "dr_util_prerequisites.h"

namespace driderSDK {

class DR_UTIL_EXPORT Random
{
 public:

  //Random between (min, max)
  template<typename type>
  static std::enable_if_t<std::is_floating_point_v<type>, type>
  get(type min, type max) 
  {
    std::uniform_real_distribution<type> dist(min, max);
    return dist(getEngine());
  }

  //Random between [min, max]
  template<typename type>
  static std::enable_if_t<std::is_integral_v<type>, type>
  get(type min, type max) 
  {
    std::uniform_int_distribution<type> dist(min, max);
    return dist(getEngine());
  }
 private:
  static std::mt19937&
  getEngine();
};

}
