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

struct DR_API_EXPORT Intersect
{
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
  static bool
  sphereSphere(const Vector3D& sph1Position,
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
  static bool
  sphereCapsule(const Vector3D& sphPosition,
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
  static bool
  capsuleCapsule(const Vector3D& capsule1A,
                 const Vector3D& capsule1B,
                 float capsule1Radius,
                 const Vector3D& capsule2A,
                 const Vector3D& capsule2B,
                 float capsule2Radius);

  /**
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
           float planeGap);

  //Uncommented
  static bool
  rayPlane(const Vector3D& rayOrigin,
           const Vector3D& rayDirection,
           const Vector3D& planeNormal,
           const Vector3D& planePoint);
  /**
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
  //Uncommented
  static bool
  rayFrustrum(const Vector3D& rayOrigin,
              const Vector3D& rayDirection,
              const std::array<Plane, 6>& frustrumPlanes);
  //Uncommented
  static bool
  rayRay(const Vector3D& rayAOrigin,
         const Vector3D& rayADirection,
         const Vector3D& rayBOrigin,
         const Vector3D& rayBDirection);
  //Uncommented
  static bool
  frustrumFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                   const std::array<Plane, 6>& frustrumBPlanes);
  //Uncommented
  static bool
  frustrumSphere(const std::array<Plane, 6>& frustrumPlanes,
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
  static bool
  aabbAabb(const Vector3D& aabbCenter,
           float aabbWidth,
           float aabbHeight,
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
  static bool
  aabbSphere(const Vector3D& aabbCenter,
             float aabbWidth,
             float aabbheight,
             const Vector3D& sphereOrigin,
             float sphereRadius);

  //Uncommented
  static bool
  aabbPlane(const Vector3D& aabbCenter,
            float aabbWidth,
            float aabbheight,
            const Vector3D& planeNormal,
            const Vector3D& planePoint);

  //Uncommented
  static bool
  aabbFrustrum(const Vector3D& aabbCenter,
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
  static bool
  aabbRay(const Vector3D& aabbCenter,
          float aabbWidth,
          float aabbHeight,
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
  static bool
  aabbPoint(const Vector3D& aabbCenter,
            float aabbWidth,
            float aabbHeight,
            const Vector3D& point);

  static bool
  spherePlane(const Vector3D& planeNormal,
              float planeGap,
              const Vector3D& sphereCenter,
              float sphereRadius);

  static bool
  aabbPlane(const Vector3D& aabbCenter,
            float aabbWidth,
            float aabbHeight,
            const Vector3D& planeNormal,
            float planeGap);

  static bool
  segmentPlane(const Vector3D& linePointA,
                         const Vector3D& linePointB,
                         const Vector3D& planeNormal,
                         float planeGap);

  static bool
  capsulePlane(const Vector3D& capsuleA,
               const Vector3D& capsuleB,
               float capsuleRadius,
               const Vector3D& planeNormal,
               float planeGap);

  static bool
  frustrumPlane(const std::array<Plane, 6>& frustrumPlanes,
                const Vector3D& planeNormal);

  /**
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

  static bool
  planePlane(const Vector3D& plane1Normal,
             const Vector3D& plane2Normal);

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
  static bool
  sphereContainsPoint(const Vector3D& point,
                      const Vector3D& sphPosition,
                      float radius);
  //Uncommented
  static bool
  frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                           const std::array<Plane, 6>& frustrumBPlanes);
  //Uncommented
  static bool
  frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
                         const Vector3D& sphereOrigin,
                         float sphereRadius);

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