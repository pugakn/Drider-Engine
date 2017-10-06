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

/**
* Check if one sphere intersects with another.
*
* @param sph1Position
*   Center of the first sphere.
*
* @param sph1Radio
*   Radius of the first sphere.
*
* @param sph2Position
*   Center of the second sphere.
*
* @param sph2Radius
*   Radius of the second sphere.
*
* @return
*   True if the point is inside, otherwise returns false.
*/
bool 
sphereSphereIntersect(const Vector3D& sph1Position,
                      float sph1Radius,
                      const Vector3D& sph2Position,
                      float sph2Radius);

/**
* Check if there was an intersection between a sphere and a capsule.
*
* @param sphPosition
*   Center of the sphere.
*
* @param sphRadio
*   Radius of the sphere.
*
* @param capsuleA
*   Point A of the capsule.
*
* @param capsuleB
*   Point B of the capsule.
*
* @param capsuleRadius
*   Radius of the capsule.
*
* @return
*   True if there is intersection, if not False
*/
bool
sphereCapsuleIntersect(const Vector3D& sphPosition,
                       float sphRadio,
                       const Vector3D& capsuleA,
                       const Vector3D& capsuleB,
                       float capsuleRadius);

/**
* Check if there was an intersection between a sphere and a capsule.
*
* @param capsule1A
*   Point A of capsule 1.
*
* @param capsule1B
*   Point B of capsule 1.
*
* @param capsule1Radius
*   Radius of capsuele 1.
*
* @param capsule2A
*   Point A of capsule 2.
*
* @param capsule2B
*   Point B of capsule 2.
*
* @param capsule2Radius
*   Radius of capsuele 2.
*
* @return
*   True if there is intersection, if not False
*/
bool
capsuleCapsuleIntersect(const Vector3D& capsule1A,
                        const Vector3D& capsule1B,
                        float capsule1Radius,
                        const Vector3D& capsule2A,
                        const Vector3D& capsule2B,
                        float capsule2Radius);

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

/**
* Check if an AABB intersects with another AABB.
*
* @param aabbCenter
*   Center of the first AABB.
*
* @param aabbWidth
*   Width of the first AABB.
*
* @param aabbHeight
*   Heigh of the first AABB.
*
* @param aabbCenter2
*   Center of the second AABB.
*
* @param aabbWidth2
*   Width of the second AABB.
*
* @param aabbHeight2
*   Heigh of the second AABB.
*
* @return
*   True if the AABB intesects, else returns false.
*/
bool
aabbAabbIntersect(const Vector3D& aabbCenter,
                  float aabbWidth,
                  float aabbheight,
                  const Vector3D& aabbCenter2,
                  float aabbWidth2,
                  float aabbheight2);

/**
* Check if a sphere intersects with an AABB.
*
* @param aabbCenter
*   Center of the AABB.
*
* @param aabbWidth
*   Width of the AABB.
*
* @param aabbheight
*   Heigh of the AABB.
*
* @param sphereOrigin
*   Origin of the sphere.
*
* @param sphereRadius
*   Radius of the sphere to check.
*
* @return
*   True if the sphere intersects, else returns false.
*/
bool
aabbSphereIntersect(const Vector3D& aabbCenter,
                    float aabbWidth,
                    float aabbheight,
                    const Vector3D& sphereOrigin,
                    float sphereRadius);

//Uncommented
bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbheight,
                   const Vector3D& planeNormal,
                   const Vector3D& planePoint);

//Uncommented
bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
                      float aabbWidth,
                      float aabbheight,
                      const std::array<Plane, 6>& frustrumPlanes);

/**
* Check if a ray intersects with an AABB.
*
* @param aabbCenter
*   Center of the AABB.
*
* @param aabbWidth
*   Width of the AABB.
*
* @param aabbHeight
*   Heigh of the AABB.
*
* @param rayOrigin
*   Origin of the ray.
*
* @param rayDirection
*   Direction of the ray.
*
* @return
*   True if the ray intersects, else returns false.
*/
bool
aabbRayIntersect(const Vector3D& aabbCenter,
                 float aabbWidth,
                 float aabbheight,
                 const Vector3D& rayOrigin,
                 const Vector3D& rayDirection);

/**
* Check if a point is inside an AABB.
*
* @param aabbCenter
*   Center of the AABB.
*
* @param aabbWidth
*   Width of the AABB.
*
* @param aabbheight
*   Heigh of the AABB.
*
* @param point
*   Point to check.
*
* @return
*   True if the point is inside, else returns false.
*/
bool
aabbPointIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbheight,
                   const Vector3D& point);


/**********************************************************************
*						            Containing functions
**********************************************************************/

/**
* Check if a point is inside a sphere.
*
* @param point
*   Point to test.
*
* @param sphPosition
*   Center of the sphere.
*
* @param radius
*   Radius of the sphere.
*
* @return
*   True if the point is inside, otherwise returns false.
*/
bool
sphereContainsPoint(const Vector3D& point,
                     const Vector3D& sphPosition,
                     float radius);

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


float 
closestPtSegmentSegment(const Vector3D& p1,
                        const Vector3D& q1,
                        const Vector3D& p2,
                        const Vector3D& q2,
                        float &s, 
                        float &t, 
                        Vector3D& c1,
                        Vector3D& c2);



}