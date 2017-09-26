#include "dr_intersections.h"

#include "dr_math.h"
#include "dr_vector3d.h"


namespace driderSDK {
/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool FORCEINLINE
pointInSphere(const Vector3D& point,
              const Vector3D& sphPosition,
              const Float32& radio)
{
  return static_cast<bool>(point.distance(sphPosition) < radio);
}
//Uncommented
bool
sphereSphereIntersection(const Vector3D& sph1Position,
                         const Float32& sph1Radio,
                         const Vector3D& sph2Position,
                         const Float32& sph2Radio)
{
 return static_cast<bool>(sph1Position.distance(sph2Position) <
        (sph1Radio + sph2Radio));
}
//Uncommented
bool
sphereRayIntersection(const Vector3D& sphPosition,
                      const Float32& sphRadio, 
                      const Vector3D& rayOrigin,
                      const Vector3D& rayDirection)
{
 if(pointInSphere(rayOrigin, sphPosition, sphRadio))
  return true;
 Vector3D vec(rayOrigin - sphPosition);

 float pDirection = vec.dot(rayDirection);

 if(pDirection < 0.0f)
  return false;

 Vector3D vecClosestPoint(rayOrigin + (rayDirection * pDirection));

 return pointInSphere(vecClosestPoint, sphPosition, sphRadio);
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

bool
aabbAabbIntersect(const Vector3D& aabb1Center,
	const Float32& aabb1Size,
	const Vector3D& aabb2Center,
	const Float32& aabb2Size)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbSphereIntersect(const Vector3D& aabbCenter,
	const Float32& aabbSize,
	const Vector3D& sphereOrigin,
	const Float32& sphereRadius)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
	const Float32& aabbSize,
	const Vector3D& planeNormal,
	const Vector3D& planePoint)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
	const Float32& aabbSize,
	const Matrix4x4& frustrumVP)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbRayIntersect(const Vector3D& aabbCenter,
	const Float32& aabbSize,
	const Vector3D& rayOrigin,
	const Vector3D& rayDirection)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
pointInAabb(const Vector3D& aabbCenter,
	const Float32& aabbSize,
	const Vector3D& point)
{
	//TODO : implement aabb intersection function
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
