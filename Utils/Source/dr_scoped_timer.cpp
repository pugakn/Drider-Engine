#include "dr_scoped_timer.h"

#include <iostream>

namespace driderSDK {

ScopedTimer::ScopedTimer() {
  m_timer.init();
}

ScopedTimer::~ScopedTimer() {
  std::cout << m_timer.getMilliseconds() << std::endl;
}

}