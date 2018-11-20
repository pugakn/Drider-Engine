#pragma once

#include "dr_timer.h"
#include "dr_util_prerequisites.h"

namespace driderSDK {

class DR_UTIL_EXPORT ScopedTimer
{
 public:
  ScopedTimer();
  ~ScopedTimer();
 private:
  Timer m_timer;
};

}
