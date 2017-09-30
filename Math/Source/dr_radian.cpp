#include <dr_radian.h>

namespace driderSDK
{

Radian::Radian() {}

Radian::Radian(const Radian& V) : m_value(V.m_value) {}

Radian::Radian(float value) : m_value(value) {}

Radian::~Radian() {}

float
Radian::toDegree() const {
  return (m_value*Math::RADIAN_TO_DEGREE);
}

Radian&
Radian::toRange() {
  while (m_value < 0 || m_value >= Math::TWO_PI)
  {
    if (m_value < 0) {
      m_value += Math::TWO_PI;
    }
    else if (m_value >= Math::TWO_PI) {
      m_value -= Math::TWO_PI;
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

Radian&
Radian::operator+=(float V)
{
  m_value += V;
  return *this;
}

Radian&
Radian::operator-=(float V)
{
  m_value -= V;
  return *this;
}

Radian&
Radian::operator*=(float V)
{
  m_value *= V;
  return *this;
}

Radian&
Radian::operator/=(float V)
{
  m_value /= V;
  return *this;
}

}