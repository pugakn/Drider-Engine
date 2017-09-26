#include <dr_degree.h>

namespace driderSDK
{

Degree::Degree() {}

Degree::Degree(const Degree& V) : m_value(V.m_value) {}

Degree::Degree(float value) : m_value(value) {}

Degree::~Degree() {}

float
Degree::toRadian() const {
  return (m_value*math::DEGREE_TO_RADIAN);
}

Degree::operator float() {
  return m_value;
}

Degree&
Degree::operator=(float V) {
  m_value = V;
  return *this;
}

}