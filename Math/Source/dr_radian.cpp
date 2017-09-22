#include <dr_radian.h>
#include <dr_degree.h>

namespace driderSDK
{

Radian::Radian()
{
}

Radian::Radian(const Radian& V) : m_value(V.m_value)
{
}

Radian::Radian(Float32 value) : m_value(value)
{
}

Radian::Radian(const Degree& V) : m_value(V.m_value)
{
}

Radian::~Radian()
{
}

Degree
Radian::toDegree() const
{
  return Degree(m_value*180/math::PI);
}

Radian::operator Float32()
{
  return m_value;
}

Radian&
Radian::operator=(Float32 V)
{
  m_value = V;
  return *this;
}

Radian&
Radian::operator=(const Degree& V)
{
  m_value = V.toRadian();
  return *this;
}

}