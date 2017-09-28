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

Radian::operator float() {
  return m_value;
}

Radian&
Radian::operator=(float V) {
  m_value = V;
  return *this;
}

}