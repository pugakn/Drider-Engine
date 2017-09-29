#include "dr_intersections.h"

#include "dr_math.h"
#include "dr_vector3d.h"
#include "dr_plane.h"

namespace driderSDK {
/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool FORCEINLINE
spherePointIntersect(const Vector3D& point,
                     const Vector3D& sphPosition,
                     float radio)
{
  return static_cast<bool>(point.distance(sphPosition) < radio);
}
//Uncommented
bool
sphereSphereIntersect(const Vector3D& sph1Position,
                      float sph1Radio,
                      const Vector3D& sph2Position,
                      float sph2Radio)
{
 return static_cast<bool>(sph1Position.distance(sph2Position) <
        (sph1Radio + sph2Radio));
}
//Uncommented
bool
sphereRayIntersect(const Vector3D& sphPosition,
                      float sphRadio, 
                      const Vector3D& rayOrigin,
                      const Vector3D& rayDirection)
{
 if(spherePointIntersect(rayOrigin, sphPosition, sphRadio))
  return true;
 Vector3D vec(rayOrigin - sphPosition);

 float pDirection = vec.dot(rayDirection);

 if(pDirection < 0.0f)
  return false;

 Vector3D vecClosestPoint(rayOrigin + (rayDirection * pDirection));

 return spherePointIntersect(vecClosestPoint, sphPosition, sphRadio);
}


bool 
rayPlaneIntersect(const Vector3D& rayOrigin, 
	                 const Vector3D& rayDirection, 
	                 const Vector3D& planeNormal, 
	                 const Vector3D& planePoint)
{
  float denom = planeNormal.dot(rayDirection);
  if (denom > Math::SMALL_NUMBER) {
  	Vector3D planeToRayOrigin = planePoint - rayOrigin;
  	float t = planeToRayOrigin.dot(planeNormal) / denom;
  	return static_cast<bool>(t >= 0);
  }
  return false;
}

bool 
rayPlaneIntersect(const Vector3D& rayOrigin, 
	              const Vector3D& rayDirection, 
	              const Vector3D& planeNormal, 
	              const Vector3D& planePoint, 
	              float* intersectionPoint)
{
  float denom = planeNormal.dot(rayDirection);
  if (denom > Math::SMALL_NUMBER) {
  	Vector3D planeToRayOrigin = planePoint - rayOrigin;
  	*intersectionPoint = planeToRayOrigin.dot(planeNormal) / denom;
  	return static_cast<bool>(*intersectionPoint >= 0);
  }
  return false;
}

bool 
rayFrustrumIntersect(const Vector3D& rayOrigin, 
	                 const Vector3D& rayDirection, 
	                 const Plane frustrumPlanes[6])
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
rayRayIntersect(const Vector3D& rayAOrigin, 
	            const Vector3D& rayADirection, 
	            const Vector3D& rayBOrigin, 
	            const Vector3D& rayBDirection)
{
	return false;
}

bool 
frustrumPlaneIntersect(const Plane frustrumPlanes[6],
	                   const Vector3D& sphereOrigin, 
	                   const Vector3D& planePoint)
{
	return false;
}

bool 
frustrumFrustrumIntersect(const Plane frustrumPlanesA[6], 
	                      const Plane frustrumPlanesB[6])
{
	return false;
}

bool 
frustrumSphereIntersect(const Plane frustrumPlanes[6],
	                    const Vector3D& sphereOrigin, 
	                    float sphereRadius)
{
	return false;
}

bool
aabbAabbIntersect(const Vector3D& aabb1Center,
                  float aabb1Size,
                  const Vector3D& aabb2Center,
                  float aabb2Size)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbSphereIntersect(const Vector3D& aabbCenter,
                    float aabbSize,
                    const Vector3D& sphereOrigin,
                    float sphereRadius)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
                   float aabbSize,
                   const Vector3D& planeNormal,
                   const Vector3D& planePoint)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
                      float aabbSize,
                      const Plane frustrumPlanes[6])
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbRayIntersect(const Vector3D& aabbCenter,
                 float aabbSize,
                 const Vector3D& rayOrigin,
                 const Vector3D& rayDirection)
{
	//TODO : implement aabb intersection function
	return false;
}

bool
aabbPointIntersect(const Vector3D& aabbCenter,
            float aabbSize,
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
	                     float sphereRadius)
{
	return false;
}

}
