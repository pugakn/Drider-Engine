#pragma once
#include <array>
#include <dr_prerequisites.h>
namespace driderSDK {

class Vector3D;
class Matrix4x4;
class Plane;

/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool
spherePointIntersect(const Vector3D& point,
                     const Vector3D& sphPosition, 
                     float radio);
//Uncommented
bool 
sphereSphereIntersect(const Vector3D& sph1Position,
                      float sph1Radio,
                      const Vector3D& sph2Position,
                      float sph2Radio);
//Uncommented
bool 
sphereRayIntersect(const Vector3D& sphPosition,
                   float sphRadio, 
                   const Vector3D& rayOrigin,
                   const Vector3D& rayDirection);

//Uncommented
bool 
rayPlaneIntersect(const Vector3D& rayOrigin,
                  const Vector3D& rayDirection, 
                  const Vector3D& planeNormal, 
                  const Vector3D& planePoint);
//Uncommented
bool
rayPlaneIntersect(const Vector3D& rayOrigin,
                  const Vector3D& rayDirection,
                  const Vector3D& planeNormal,
                  const Vector3D& planePoint,
                  float* intersectionPoint);

//Uncommented
bool 
rayFrustrumIntersect(const Vector3D& rayOrigin, 
                     const Vector3D& rayDirection, 
                     const std::array<Plane,6>& frustrumPlanes);
//Uncommented
bool
rayRayIntersect(const Vector3D& rayAOrigin,
                   const Vector3D& rayADirection,
                   const Vector3D& rayBOrigin,
                   const Vector3D& rayBDirection);

//Uncommented
bool
frustrumPlaneIntersect(const std::array<Plane, 6>& frustrumPlanes,
                       const Vector3D& sphereOrigin,
                       const Vector3D& planePoint);
//Uncommented
bool
frustrumFrustrumIntersect(const std::array<Plane,6>& frustrumAPlanes,
                          const std::array<Plane,6>& frustrumBPlanes);
//Uncommented
bool
frustrumSphereIntersect(const std::array<Plane, 6>& frustrumPlanes,
                        const Vector3D& sphereOrigin,
                        float sphereRadius);
//Uncommented
//static bool
//frustrumCapsuleIntersection(const Matrix4x4& frustrumVP);

//Uncommented
bool
aabbAabbIntersect(const Vector3D& aabb1Center,
                  float aabb1Size,
                  const Vector3D& aabb2Center,
                  float aabb2Size);

//Uncommented
bool
aabbSphereIntersect(const Vector3D& aabbCenter,
                    float aabbSize,
                    const Vector3D& sphereOrigin,
                    float sphereRadius);

//Uncommented
bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
                   float aabbSize, 
                   const Vector3D& planeNormal,
                   const Vector3D& planePoint);

//Uncommented
bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
                      float aabbSize,
                      const std::array<Plane, 6>& frustrumPlanes);

//Uncommented
bool
aabbRayIntersect(const Vector3D& aabbCenter,
                 float aabbSize, 
                 const Vector3D& rayOrigin,
                 const Vector3D& rayDirection);

//Uncommented
bool
aabbPointIntersect(const Vector3D& aabbCenter,
                   float aabbSize, 
                   const Vector3D& point);


/**********************************************************************
*						            Containing functions
**********************************************************************/

//Uncommented
bool
frustrumContainsPlane(std::array<Plane, 6>& frustrumPlanes,
	                     const Vector3D& planeNormal,
	                     const Vector3D& planePointD);
//Uncommented
bool
frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
	                        const std::array<Plane, 6>& frustrumBPlanes);
//Uncommented
bool
frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
	                     const Vector3D& sphereOrigin,
	                     float sphereRadius);





}