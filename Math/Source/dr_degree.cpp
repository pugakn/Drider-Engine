#include <dr_degree.h>
#include <dr_radian.h>

namespace driderSDK {

Degree::Degree()
{
}

Degree::Degree(const Degree& V) : m_value(V.m_value)
{
}

Degree::Degree(Float32 value) : m_value(value)
{
}

Degree::Degree(const Radian& V) : m_value(V.m_value)
{
}

Degree::~Degree()
{
}

Radian
Degree::toRadian() const
{
  return Radian(m_value*math::PI/180);
}

Degree::operator Float32()
{
  return m_value;
}

Degree&
Degree::operator=(Float32 V)
{
  m_value = V;
  return *this;
}

Degree&
Degree::operator=(const Radian& V)
{
  m_value = V.toDegree();
  return *this;
}

}