#include "dr_intersections.h"

#include "dr_math.h"
#include "dr_sphere.h"
#include "dr_capsule.h"
#include "dr_ray.h"
#include "dr_vector3d.h"


namespace driderSDK {
/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool PointInSphere(
  const Vector3D& vec,
  const Sphere& s1)
{
  return s1.getCenter().distance(vec) < s1.getRadio();
}
//Uncommented
bool CollSphereSphere(
  const Sphere& s1, 
  const Sphere& s2)
{
 return s1.getCenter().distance(s2.getCenter()) <
        (s1.getRadio() + s2.getRadio());
}

//Uncommented
bool CollSphereCapsule(
  const Sphere& s1,
  const Capsule& c1)
{
  return false;
}
//Uncommented
bool CollSphereRay(
  const Sphere& s1,
  const Ray& r1)
{
  if(PointInSphere(r1.getOrigin(), s1))
    return true;
  Vector3D vec(r1.getOrigin() - s1.getCenter());

  float pDirection = vec.dot(r1.getDirection());

  if(pDirection < 0.0f)
    return false;

  Vector3D vecClosestPoint(r1.getOrigin() + (r1.getDirection() * pDirection));

  return PointInSphere(vecClosestPoint, s1);
}


bool 
rayPlaneIntersection(const Vector3D& rayOrigin, 
	                   const Vector3D& rayDirection, 
	                   const Vector3D& planeNormal, 
	                   const Vector3D& planePoint)
{
	Float32 denom = planeNormal.dot(rayDirection);
	if (denom > math::SMALL_NUMBER) {
		Vector3D planeToRayOrigin = planePoint - rayOrigin;
		Float32 t = planeToRayOrigin.dot(planeNormal) / denom;
		return static_cast<bool>(t >= 0);
	}
	return false;
}

bool 
rayPlaneIntersection(const Vector3D& rayOrigin, 
	                   const Vector3D& rayDirection, 
	                   const Vector3D& planeNormal, 
	                   const Vector3D& planePoint, 
	                   Float32* intersectionPoint)
{
	Float32 denom = planeNormal.dot(rayDirection);
	if (denom > math::SMALL_NUMBER) {
		Vector3D planeToRayOrigin = planePoint - rayOrigin;
		*intersectionPoint = planeToRayOrigin.dot(planeNormal) / denom;
		return static_cast<bool>(*intersectionPoint >= 0);
	}
	return false;
}

bool 
rayFrustrumIntersection(const Vector3D& rayOrigin, 
	                      const Vector3D& rayDirection, 
	                      const Matrix4x4& frustrumVP)
{
	return false;
}

//bool 
//rayCapsuleIntersection(const Vector3D& rayOrigin, 
//	                     const Vector3D& rayDirection, 
//	                     const Matrix4x4& frustrumVP)
//{
//	return false;
//}

bool 
rayRayIntersection(const Vector3D& rayAOrigin, 
	                 const Vector3D& rayADirection, 
	                 const Vector3D& rayBOrigin, 
	                 const Vector3D& rayBDirection)
{
	return false;
}

bool 
frustrumPlaneIntersection(const Matrix4x4& frustrumVP, 
	                        const Vector3D& sphereOrigin, 
	                        const Vector3D& planePoint)
{
	return false;
}

bool 
frustrumFrustrumIntersection(const Matrix4x4& frustrumAVP, 
	                           const Matrix4x4& frustrumBVP)
{
	return false;
}

bool 
frustrumSphereIntersection(const Matrix4x4& frustrumVP, 
	                         const Vector3D& sphereOrigin, 
	                         const Float32& sphereRadius)
{
	return false;
}

/**********************************************************************
*						            Containing functions
**********************************************************************/
bool FORCEINLINE
frustrumContainsPlane(const Matrix4x4& frustrumVP, 
	                    const Vector3D& planeNormal, 
	                    const Vector3D& planePoint)
{
	return frustrumPlaneIntersection(frustrumVP, planeNormal, planePoint);
}

bool 
frustrumContainsFrustrum(const Matrix4x4& frustrumAVP, 
	                       const Matrix4x4& frustrumBVP)
{
	return false;
}

bool 
frustrumContainsSphere(const Matrix4x4& frustrumVP, 
	                     const Vector3D& sphereOrigin, 
	                     const Float32& sphereRadius)
{
	return false;
}

}
