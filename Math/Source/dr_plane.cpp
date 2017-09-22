#include <dr_plane.h>

namespace driderSDK {

Plane::Plane()
{}

Plane::Plane(const Vector3D & _normal, Float32 _d)
  : m_normal(_normal),
	m_d(_d)
{}

Plane::Plane(const Vector3D & _normal, const Vector3D & _point)
{
  m_normal = _normal;
  m_d = -_point.dot(m_normal);
}

Plane::Plane(const Vector3D & point0, 
			 const Vector3D & point1, 
			 const Vector3D & point2)
{
  m_normal = (point1 - point0).cross(point2 - point0).normalize();
  m_d = -point0.dot(m_normal);
}

Plane::Plane(const Plane & other)
  : m_normal(other.m_normal),
	m_d(other.m_d)
{}

Float32 Plane::distanceToPoint(const Vector3D & point)
{
  /******Formula*******
   	 N = Plane Normal 
   	 P = Point		  
   	 d = Plane gap

   	 |dot(N, P) + d|  
   	 ---------------  
      sqrt(dot(N,N))  
  *********************/

  //since N is normalized sqrt(dot(N,N)) == 1 there
  //is no need for division

  return math::abs(point.dot(m_normal) + m_d);
}

Vector3D Plane::getNormal() const
{
  return m_normal;
}

Float32 Plane::getGap() const
{
  return m_d;
}

Plane& Plane::operator=(const Plane & other)
{
  m_normal = other.m_normal;
  m_d = other.m_d;
}

bool Plane::operator==(const Plane & rhs)
{
  return m_normal == rhs.m_normal && m_d == rhs.m_d;
}

bool Plane::operator!=(const Plane & rhs)
{
  return !(*this == rhs);
}

}