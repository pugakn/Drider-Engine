#include <dr_degree.h>

namespace driderSDK
{

Degree::Degree() {}

Degree::Degree(const Degree& V) : m_value(V.m_value) {}

Degree::Degree(float value) : m_value(value) {}

Degree::~Degree() {}

float
Degree::toRadian() const {
  return (m_value*Math::DEGREE_TO_RADIAN);
}

Degree&
Degree::toRange() {
  while (m_value < 0 || m_value >= 360)
  {
    if (m_value < 0) {
      m_value += 360;
    }
    else if (m_value >= 360) {
      m_value -= 360;
    }
  }
  return *this;
}

Degree::operator float() {
  return m_value;
}

Degree&
Degree::operator=(float V) {
  m_value = V;
  return *this;
}

Degree&
Degree::operator+=(float V)
{
  m_value += V;
  return *this;
}

Degree&
Degree::operator-=(float V)
{
  m_value -= V;
  return *this;
}

Degree&
Degree::operator*=(float V)
{
  m_value *= V;
  return *this;
}

Degree&
Degree::operator/=(float V)
{
  m_value /= V;
  return *this;
}

}