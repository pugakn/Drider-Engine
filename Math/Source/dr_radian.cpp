#include <dr_radian.h>

namespace driderSDK
{

Radian::Radian() {}

Radian::Radian(const Radian& V) : m_value(V.m_value) {}

Radian::Radian(float value) : m_value(value) {}

Radian::~Radian() {}

float
Radian::toDegree() const {
  return (m_value*math::RADIAN_TO_DEGREE);
}

Radian&
Radian::toRange() {
  while (m_value < 0 || m_value >= math::TWO_PI)
  {
    if (m_value < 0) {
      m_value += math::TWO_PI;
    }
    else if (m_value >= math::TWO_PI) {
      m_value -= math::TWO_PI;
    }
  }
  return *this;
}

Radian::operator float() {
  return m_value;
}

Radian&
Radian::operator=(float V) {
  m_value = V;
  return *this;
}

}