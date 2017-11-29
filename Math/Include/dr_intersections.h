#pragma once

#include <array>
#include "dr_math_prerequisites.h"

namespace driderSDK {

class Vector3D;
class Matrix4x4;
class Plane;

/**********************************************************************
*						            Intersection functions
**********************************************************************/

namespace PLANE_INTERSECT {
//Enum used to describe the relationship between a point
//and a plane.
enum E
{
  //The point is in front of the plane.
  kFront,
  //The point is in the plane.
  kIn,
  //The point is behind plane.
  kBehind,
};
}

struct DR_MATH_EXPORT Intersect
{
  /**
  * TEST::sphereSphere
  * Checks if one sphere intersects with another.
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
  static bool
  sphereSphere(const Vector3D& sph1Position,
               float sph1Radius,
               const Vector3D& sph2Position,
               float sph2Radius);

  /**
  * TEST::sphereCapsule
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
  static bool
  sphereCapsule(const Vector3D& sphPosition,
                float sphRadio,
                const Vector3D& capsuleA,
                const Vector3D& capsuleB,
                float capsuleRadius);
  /**
  * TEST::capsuleCapsule
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
  static bool
  capsuleCapsule(const Vector3D& capsule1A,
                 const Vector3D& capsule1B,
                 float capsule1Radius,
                 const Vector3D& capsule2A,
                 const Vector3D& capsule2B,
                 float capsule2Radius);

  /**
  * TEST::sphereRay
  * Check if there was an intersection between a sphere and a capsule.
  *
  * @param sphPosition
  *   Center of the sphere to check.
  *
  * @param sphRadio
  *   Radius of the sphere.
  *
  * @param rayOrigin
  *   Origin of the sphere.
  *
  * @param rayDirection
  *   Directions of the ray.
  *
  * @return
  *   True if there is intersection, if not False
  */
  static bool
  sphereRay(const Vector3D& sphPosition,
            float sphRadio,
            const Vector3D& rayOrigin,
            const Vector3D& rayDirection);

  /**
  * TEST::sphereRay
  * Check if there was an intersection between a sphere and a capsule.
  *
  * @param sphPosition
  *   Center of the sphere to check.
  *
  * @param sphRadio
  *   Radius of the sphere.
  *
  * @param rayOrigin
  *   Origin of the sphere.
  *
  * @param rayDirection
  *   Directions of the ray.
  *
  * @return
  *   True if there is intersection, if not False
  */
  static bool
  sphereRay(const Vector3D& sphPosition,
            float sphRadio,
            const Vector3D& rayOrigin,
            const Vector3D& rayDirection,
            Vector3D* intersectionPoint);

  /**
  * TEST::NoImplemented
  */
  static bool
  rayPlane(const Vector3D& rayOrigin,
           const Vector3D& rayDirection,
           const Vector3D& planeNormal,
           float planeGap);

  /**
  * TEST::rayPlane
  * Check if there was an intersection between a sphere and a capsule.
  *
  * @param rayOrigin
  *   Origin of the ray.
  *
  * @param rayDirection
  *   Direction of the ray.
  *
  * @param planeNormal
  *   Normal vector of the plane.
  *
  * @param planePoint
  *   Point on the plane.
  *
  * @return
  *   True if there is intersection, if not False
  */
  static bool
  rayPlane(const Vector3D& rayOrigin,
           const Vector3D& rayDirection,
           const Vector3D& planeNormal,
           const Vector3D& planePoint);
  /**
  * TEST::rayPlane
  * Check if there was an intersection between a sphere and a capsule.
  *
  * @param rayOrigin
  *   Origin of the ray.
  *
  * @param rayDirection
  *   Direction of the ray.
  *
  * @param planeNormal
  *   Normal vector of the plane.
  *
  * @param planePoint
  *   Point on the plane.
  *
  * @param intersectionPoint
  *   Return the intersect point.
  *
  * @return
  *   True if there is intersection, if not False
  */
  static bool
  rayPlane(const Vector3D& rayOrigin,
           const Vector3D& rayDirection,
           const Vector3D& planeNormal,
           const Vector3D& planePoint,
           float* intersectionPoint);


  /**
  * TEST::rayCapsule
  * Check if there was an intersection between a sphere and a capsule.
  *
  * @param pointSA
  *   Min point of the capsule.
  *
  * @param pointSB
  *   Max point of the capsule.
  *
  * @param pointP
  *   Origin of the ray.
  *
  * @param pointQ
  *   Final point of the ray.
  *
  * @param r
  *   Radius of the capsule.
  *
  * @param t
  *   Range of intersection.
  *
  * @return
  *   True if there is intersection, if not False
  */
  static bool
  rayCapsule(const Vector3D& pointSA,
             const Vector3D& pointSB,
             const Vector3D& pointP,
             const Vector3D& pointQ,
             float r,
             float &t);

  static bool
  rayFrustrum(const Vector3D& rayOrigin,
              const Vector3D& rayDirection,
              const std::array<Plane, 6>& frustrumPlanes);
  /**
  * TEST::frustrumFrustrum
  * Check if there was an intersection between a frustrum and other frustrum.
  *
  * @param frustrumAPlanes
  *   Planes of the first frustrum.
  *
  * @param frustrumBPlanes
  *   Planes of the second frustrum.
  *
  * @return
  *   True if there is intersection, if not false
  */
  static bool
  frustrumFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                   const std::array<Plane, 6>& frustrumBPlanes);

  /**
  * TEST::frustrumSphere
  * Check if there was an intersection between a frustrum and a sphere.
  *
  * @param frustrumPlanes
  *   Planes of the frustrum.
  *
  * @param sphereOrigin
  *   Origin of the sphere.
  *
  * @return
  *   True if there is intersection, if not false
  */
  static bool
  frustrumSphere(const std::array<Plane, 6>& frustrumPlanes,
                 const Vector3D& sphereOrigin,
                 float sphereRadius);


  static bool
  frustrumCapsuleIntersection(const Matrix4x4& frustrumVP);

  /**
  * TEST::aabbAabb
  * Check if an AABB intersects with another AABB.
  *
  * @param aabbCenter
  *   Center of the first AABB.
  *
	* @param aabbWidth
	*   Width of the AABB
	*
	* @param aabbHeight
	*   Height of the AABB
	*
	* @param aabbDepth
	*   Depth of the AABB
  *
  * @param aabbCenter2
  *   Center of the second AABB.
  *
	* @param aabbWidth2
	*   Width of the other AABB
	*
	* @param aabbHeight2
	*   Height of the other AABB
	*
	* @param aabbDepth2
	*   Depth of the other AABB
  *
  * @return
  *   True if the AABB intesects, else returns false.
  */
  static bool
  aabbAabb(const Vector3D& aabbCenter,
					 float aabbWidth,
					 float aabbHeight,
					 float aabbDepth,
           const Vector3D& aabbCenter2,
					 float aabbWidth2,
					 float aabbHeight2,
					 float aabbDepth2);

  /**
  * TEST::aabbSphere
  * Check if a sphere intersects with an AABB.
  *
  * @param aabbCenter
  *   Center of the AABB.
  *
	* @param aabbWidth
	*   Width of the AABB
	*
	* @param aabbHeight
	*   Height of the AABB
	*
	* @param aabbDepth
	*   Depth of the AABB
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
  static bool
  aabbSphere(const Vector3D& aabbCenter,
						 float aabbWidth,
						 float aabbHeight,
						 float aabbLength,
             const Vector3D& sphereOrigin,
             float sphereRadius);

  /**
  * TEST::NoImplemented
  * Check if a ray intersects with an AABB.
  *
  * @param aabbCenter
  *   Center of the AABB.
  *
	* @param aabbWidth
	*   Width of the AABB
  *
	* @param aabbHeight
	*   Height of the AABB
	*
	* @param aabbDepth
	*   Depth of the AABB
	*
  * @param frustrumPlanes
  *   Planes of the frustrum.
  *
  * @return
  *   True if the ray intersects, else returns false.
  */
  static bool
  aabbFrustrum(const Vector3D& aabbCenter,
							 float aabbWidth,
							 float aabbHeight,
							 float aabbDepth,
               const std::array<Plane, 6>& frustrumPlanes);

  /**
  * TEST::aabbRay
  * Check if a ray intersects with an AABB.
  *
  * @param aabbCenter
  *   Center of the AABB.
  *
	* @param aabbMax
	*   Vector3D containing maximum point of the AABB
	*
	* @param aabbMin
	*   Vector3D containing minimum point of the AABB
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
  static bool
  aabbRay(const Vector3D& aabbCenter,
					const Vector3D& aabbMax,
					const Vector3D& aabbMin,
          const Vector3D& rayOrigin,
          const Vector3D& rayDirection);

  static bool
  rayAABB(const Vector3D& aabbMax,
					const Vector3D& aabbMin,
          const Vector3D& rayOrigin,
          const Vector3D& rayDirection,
          Vector3D* point);

  /**
  * TEST::aabbPoint
  * Check if a point is inside an AABB.
  *
  * @param aabbCenter
  *   Center of the AABB.
  *
	* @param aabbMax
	*   Vector3D containing maximum point of the AABB
	*
	* @param aabbMin
	*   Vector3D containing minimum point of the AABB
  *
  * @param point
  *   Point to check.
  *
  * @return
  *   True if the point is inside, else returns false.
  */
  static bool
  aabbPoint(const Vector3D& aabbCenter,
						const Vector3D& aabbMax,
						const Vector3D& aabbMin,
            const Vector3D& point);

  /**
  * TEST::spherePlane
  * Check if a sphere intersects with a plane.
  *
  * @param planeNormal
  *   Normal of the plane.
  *
  * @param planeGap
  *   Gap of the plane.
  *
  * @param sphereCenter
  *   Center of the sphere.
  *
  * @param sphereRadius
  *   Radius of the sphere.
  *
  * @return
  *   True if the plane intersects with plane, else returns false.
  */
  static bool
  spherePlane(const Vector3D& planeNormal,
              float planeGap,
              const Vector3D& sphereCenter,
              float sphereRadius);

  /**
  * TEST::aabbPlane
  * Check if a aabb intersects with a plane.
  *
  * @param aabbCenter
  *   Center of Aabb.
  *
	* @param aabbWidth
	*   Width of the AABB
	*
	* @param aabbHeight
	*   Height of the AABB
	*
	* @param aabbDepth
	*   Depth of the AABB
  *
  * @param planeNormal
  *   Normal of the plane.
  *
  * @param planeGap
  *  Gap of the plane.
  *
  * @return
  *   True if the aabb intersects with a plane, else returns false.
  */
  static bool
  aabbPlane(const Vector3D& aabbCenter,
						float aabbWidth,
						float aabbHeight,
						float aabbDepth,
            const Vector3D& planeNormal,
            float planeGap);

  /**
  * TEST::segmentPlane
  * Check if a segment intersects a plane.
  *
  * @param linePointA
  *   Origin of the line.
  *
  * @param linePointB
  *   End of the line.
  *
  * @param aabbHeight
  *   Height of Aabb.
  *
  * @param planeNormal
  *   Normal of the plane.
  *
  * @param planeGap
  *  Gap of the plane.
  *
  * @return
  *   True if the segmen intersects with a plane, else returns false.
  */
  static bool
  segmentPlane(const Vector3D& linePointA,
                         const Vector3D& linePointB,
                         const Vector3D& planeNormal,
                         float planeGap);
  /**
  * TEST::capsulePlane
  * Check if a capsule intersects with a plane.
  *
  * @param capsuleA
  *   Point a of the capsule.
  *
  * @param capsuleB
  *   Point b of the capsule.
  *
  * @param capsuleRadius
  *   Capsule's radius.
  *
  * @param planeNormal
  *   Normal of the plane.
  *
  * @param planeGap
  *  Gap of the plane.
  *
  * @return
  *   True if the capsule intersects with a plane, else returns false.
  */
  static bool
  capsulePlane(const Vector3D& capsuleA,
               const Vector3D& capsuleB,
               float capsuleRadius,
               const Vector3D& planeNormal,
               float planeGap);

  /**
  * TEST::frustrumPlane
  * Check if a frustrum intersects with a plane.
  *
  * @param frustrumPlanes
  *   Frustrum's planes.
  *
  * @param planeNormal
  *   Plane's normal.
  *
  * @return
  *   True if the frustrum intersects with a plane, else returns false.
  */
  static bool
  frustrumPlane(const std::array<Plane, 6>& frustrumPlanes,
                const Vector3D& planeNormal);

  /**
  * TEST::planePlane
  * Checks if there is an intersection between 2 planes.
  *
  * @param plane1Normal
  *  Normal of the first plane.
  *
  * @parama plane1Gap
  *  Gap of the first plane.
  *
  * @param plane2Normal
  *  Normal of the second plane.
  *
  * @param plane2Gap
  *  Gap of the second plane.
  *
  * @out point
  *  A point of the intersection line.
  *
  * @out direction
  *  Direction of the instersection line.
  *
  * @return
  *   True if the planes intersect, false if they are
  *   parallel (and separated) or coincident.
  */
  static bool
  planePlane(const Vector3D& plane1Normal,
             float plane1Gap,
             const Vector3D& plane2Normal,
             float plane2Gap,
             Vector3D& point,
             Vector3D& direction);
  
  /**
  * TEST::planePlane
  * Checks if there is an intersection between 2 planes.
  *
  * @param plane1Normal
  *  Normal of the first plane.
  *
  * @parama plane2Normal
  *  Gap of the first plane.
  *
  * @return
  *   True if the planes intersect, false if they are
  *   parallel (and separated) or coincident.
  */
  static bool
  planePlane(const Vector3D& plane1Normal,
             const Vector3D& plane2Normal);

  /**********************************************************************
  *						            Containing functions
  **********************************************************************/

  /**
  * TEST::sphereContainsPoint
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
  static bool
  sphereContainsPoint(const Vector3D& point,
                      const Vector3D& sphPosition,
                      float radius);
  /*//Uncommented
  static bool
  frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                           const std::array<Plane, 6>& frustrumBPlanes);
  //Uncommented
  static bool
  frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
                         const Vector3D& sphereOrigin,
                         float sphereRadius);*/
  /**
  * TEST::closestPointSegmentSegment
  * Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
  * S2(t)=P2+t*(Q2-P2).
  *
  * @param p1
  *   Origing segment 1.
  *
  * @param q1
  *   Final point segment 1.
  *
  * @param p2
  *   Origing segment 2.
  *
  * @param q2
  *   Final point segment 2.
  *
  * @param s
  *   Segment final.
  *
  * @param t
  *   Intersect interval.
  *
  * @param c1
  *   Intersect point 1.
  *
  * @param c2
  *   Intersect point 2.
  *
  * @return
  *   Returning s and t. Function result is squared distance
  *   between between S1(s) and S2(t).
  */
  static float
  closestPointSegmentSegment(const Vector3D& p1,
                             const Vector3D& q1,
                             const Vector3D& p2,
                             const Vector3D& q2,
                             float &s,
                             float &t,
                             Vector3D& c1,
                             Vector3D& c2);
};

}