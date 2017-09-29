#include <dr_plane.h>

namespace driderSDK {

Plane::Plane()
{}

Plane::Plane(const Vector3D & _normal, float _d)
  : Vector3D(_normal),
	  d(_d)
{}

Plane::Plane(const Vector3D & _normal, const Vector3D & _point)
{
  Vector3D::operator=(_normal);
  d = -dot(_point);
}

Plane::Plane(const Vector3D & point0, 
			 const Vector3D & point1, 
			 const Vector3D & point2)
{
  Vector3D::operator=(point1 - point0).cross(point2 - point0);
  normalize();
  d = -dot(point0);
}

Plane::Plane(const Plane & other)
  : Vector3D(other),
	  d(other.d)
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

  return Math::abs(dot(point) + d) / length();
}

float Plane::signedDistanceToPoint(const Vector3D & point)
{
  return (dot(point) + d)  / length();
}

bool Plane::intersects(const Vector3D & point)
{
  //Ax+By+Cz = D
  //Ax + By + Cz - D = 0 If this is true the point is in the plane

  //Little presicion error 
  float error = 0.0001f;

  float res = dot(point) - d;

  return Math::abs(res) < Math::EPSILON;  
}

void Plane::normalize()
{
  float invLength = 1.f / length();
}

Plane& Plane::operator=(const Plane & other)
{
  Vector3D::operator=(other);
  d = other.d;

  return *this;
}

bool Plane::operator==(const Plane& rhs)
{
  return Vector3D::operator==(rhs) && d == rhs.d;
}

bool Plane::operator!=(const Plane & rhs)
{
  return !(*this == rhs);
}

}