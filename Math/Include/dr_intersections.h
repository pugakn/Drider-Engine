#pragma once
#include <dr_prerequisites.h>
namespace driderSDK {

class Vector3D;
class Matrix4x4;


/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool
pointInSphere(const Vector3D& point, 
             const Vector3D& sphPosition, 
             const Float32& radio);
//Uncommented
bool 
sphereSphereIntersection(const Vector3D& sph1Position,
                         const Float32& sph1Radio,
                         const Vector3D& sph2Position,
                         const Float32& sph2Radio);
//Uncommented
bool 
sphereRayIntersection(const Vector3D& sphPosition,
                      const Float32& sphRadio, 
                      const Vector3D& rayOrigin,
                      const Vector3D& rayDirection);

//Uncommented
bool 
rayPlaneIntersection(const Vector3D& rayOrigin, 
										 const Vector3D& rayDirection, 
										 const Vector3D& planeNormal, 
										 const Vector3D& planePoint);
//Uncommented
bool
rayPlaneIntersection(const Vector3D& rayOrigin,
	                   const Vector3D& rayDirection,
	                   const Vector3D& planeNormal,
	                   const Vector3D& planePoint,
	                   Float32* intersectionPoint);

//Uncommented
bool 
rayFrustrumIntersection(const Vector3D& rayOrigin, 
												const Vector3D& rayDirection, 
												const Matrix4x4& frustrumVP);
//Uncommented
bool
rayRayIntersection(const Vector3D& rayAOrigin,
	                 const Vector3D& rayADirection,
	                 const Vector3D& rayBOrigin,
	                 const Vector3D& rayBDirection);

//Uncommented
bool
frustrumPlaneIntersection(const Matrix4x4& frustrumVP,
	                        const Vector3D& sphereOrigin,
	                        const Vector3D& planePoint);
//Uncommented
bool
frustrumFrustrumIntersection(const Matrix4x4& frustrumAVP,
	                           const Matrix4x4& frustrumBVP);
//Uncommented
bool
frustrumSphereIntersection(const Matrix4x4& frustrumVP,
	                         const Vector3D& sphereOrigin,
	                         const Float32& sphereRadius);
//Uncommented
//static bool
//frustrumCapsuleIntersection(const Matrix4x4& frustrumVP);

//Uncommented
bool
aabbAabbIntersect(const Vector3D& aabb1Center,
				    const Float32& aabb1Size,
					const Vector3D& aabb2Center,
					const Float32& aabb2Size);

//Uncommented
bool
aabbSphereIntersect(const Vector3D& aabbCenter,
					  const Float32& aabbSize,
					  const Vector3D& sphereOrigin,
					  const Float32& sphereRadius);

//Uncommented
bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
				     const Float32& aabbSize, 
					 const Vector3D& planeNormal,
					 const Vector3D& planePoint);

//Uncommented
bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
					    const Float32& aabbSize,
					    const Matrix4x4& frustrumVP);

//Uncommented
bool
aabbRayIntersect(const Vector3D& aabbCenter,
				   const Float32& aabbSize, 
				   const Vector3D& rayOrigin,
				   const Vector3D& rayDirection);

//Uncommented
bool
pointInAabb(const Vector3D& aabbCenter,
			  const Float32& aabbSize, 
			  const Vector3D& point);


/**********************************************************************
*						            Containing functions
**********************************************************************/

//Uncommented
bool
frustrumContainsPlane(const Matrix4x4& frustrumVP,
	                    const Vector3D& planeNormal,
	                    const Vector3D& planePointD);
//Uncommented
bool
frustrumContainsFrustrum(const Matrix4x4& frustrumAVP,
	                       const Matrix4x4& frustrumBVP);
//Uncommented
bool
frustrumContainsSphere(const Matrix4x4& frustrumVP,
	                     const Vector3D& sphereOrigin,
	                     const Float32& sphereRadius);





}