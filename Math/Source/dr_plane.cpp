#include <dr_plane.h>

namespace driderSDK {

Plane::Plane()
{}

Plane::Plane(const Vector3D & _normal, float _d)
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

float Plane::distanceToPoint(const Vector3D & point)
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

  return math::abs(point.dot(m_normal) + m_d) / m_normal.length();
}

float Plane::signedDistanceToPoint(const Vector3D & point)
{
  return (point.dot(m_normal) + m_d)  / m_normal.length();
}

bool Plane::intersects(const Vector3D & point)
{
  //Ax+By+Cz = D
  //Ax + By + Cz - D = 0 If this is true the point is in the plane

  //Little presicion error 
  float error = 0.0001f;

  float res = m_normal.dot(point) - m_d;

  return math::abs(res) < error;  
}

Plane Plane::normalize() const
{
  float invLength = 1.f / m_normal.length();
  
  return Plane(m_normal * invLength, m_d * invLength);
}

Vector3D Plane::getNormal() const
{
  return m_normal;
}

float Plane::getGap() const
{
  return m_d;
}

Plane& Plane::operator=(const Plane & other)
{
  m_normal = other.m_normal;
  m_d = other.m_d;
  return *this;
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