#pragma once
#include <dr_prerequisites.h>
namespace driderSDK {

class Sphere;
class Capsule;
class Ray;
class Vector3D;
class Frustrum;
class Plane;
class Matrix4x4;


/**********************************************************************
*						            Intersection functions
**********************************************************************/
//Uncommented
bool 
PointInSphere(const Vector3D& vec, const Sphere& s1);
//Uncommented
bool 
CollSphereSphere(const Sphere& s1, const Sphere& s2);
//Uncommented
bool 
CollSphereCapsule(const Sphere& s1, const Capsule& c1);
//Uncommented
bool 
CollSphereRay(const Sphere& s1, const Ray& r1);

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
//static bool
//rayCapsuleIntersection(const Vector3D& rayOrigin,
//										   const Vector3D& rayDirection,
//	                     const Matrix4x4& frustrumVP);
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