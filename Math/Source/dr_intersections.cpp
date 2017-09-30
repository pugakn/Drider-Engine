#include "dr_intersections.h"
#include "dr_math.h"
#include "dr_vector3d.h"
#include "dr_plane.h"

namespace driderSDK {
/**********************************************************************
*						            Intersection functions
**********************************************************************/

bool
sphereSphereIntersect(const Vector3D& sph1Position,
                      float sph1Radius,
                      const Vector3D& sph2Position,
                      float sph2Radius)
{
  return static_cast<bool>(sph1Position.distance(sph2Position) <
         (sph1Radius + sph2Radius));
}

bool
sphereCapsuleIntersect(const Vector3D& sphPosition,
                       float sphRadio,
                       const Vector3D& capsuleA,
                       const Vector3D& capsuleB,
                       float capsuleRadius)
{
  float dist = sphPosition.sqrDistSegment(capsuleA, capsuleB);

  float radius = sphRadio + capsuleRadius;
  return dist <= radius * radius;
}

bool
capsuleCapsuleIntersect(const Vector3D& capsule1A,
                        const Vector3D& capsule1B,
                        const Vector3D& capsule1Radius,
                        const Vector3D& capsule2A,
                        const Vector3D& capsule2B,
                        const Vector3D& capsule2Radius)
{
  // I need ClosestPtSegmentSegment, but i don't know where i put it.
  return false;
}
bool
sphereRayIntersect(const Vector3D& sphPosition,
                   float sphRadio,
                   const Vector3D& rayOrigin,
                   const Vector3D& rayDirection)
{
  if (sphereContainsPoint(rayOrigin, sphPosition, sphRadio)) {
    return true;
  }
  Vector3D vec(rayOrigin - sphPosition);

  float pDirection = vec.dot(rayDirection);

  if (pDirection < 0.0f) {
    return false;
  }

  Vector3D vecClosestPoint(rayOrigin + (rayDirection * pDirection));

  return sphereContainsPoint(vecClosestPoint, sphPosition, sphRadio);
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
                     const std::array<Plane, 6>& frustrumPlanes)
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
frustrumPlaneIntersect(const std::array<Plane, 6>& frustrumPlanes,
                       const Vector3D& sphereOrigin,
                       const Vector3D& planePoint)
{
  return false;
}

bool
frustrumFrustrumIntersect(const std::array<Plane, 6>& frustrumAPlanes,
                          const std::array<Plane, 6>& frustrumBPlanes)
{
  return false;
}

bool
frustrumSphereIntersect(const std::array<Plane, 6>& frustrumPlanes,
                        const Vector3D& sphereOrigin,
                        float sphereRadius)
{
  return false;
}

bool
aabbAabbIntersect(const Vector3D& aabb1Center,
                  float aabbWidth,
                  float aabbheight,
                  const Vector3D& aabb2Center,
                  float aabbWidth2,
                  float aabbheight2)
{
  //TODO : implement aabb intersection function
  return false;
}

bool
aabbSphereIntersect(const Vector3D& aabbCenter,
                    float aabbWidth,
                    float aabbheight,
                    const Vector3D& sphereOrigin,
                    float sphereRadius)
{
  //TODO : implement aabb intersection function
  return false;
}

bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbheight,
                   const Vector3D& planeNormal,
                   const Vector3D& planePoint)
{
  //TODO : implement aabb intersection function
  return false;
}

bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
                      float aabbWidth,
                      float aabbheight,
                      const std::array<Plane, 6>& frustrumPlanes)
{
  //TODO : implement aabb intersection function
  return false;
}

bool
aabbRayIntersect(const Vector3D& aabbCenter,
                 float aabbWidth,
                 float aabbheight,
                 const Vector3D& rayOrigin,
                 const Vector3D& rayDirection)
{
  //TODO : implement aabb intersection function
  return false;
}

bool
aabbPointIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbheight,
                   const Vector3D& point)
{
  //TODO : implement aabb intersection function
  return false;
}

/**********************************************************************
*						            Containing functions
**********************************************************************/
bool
sphereContainsPoint(const Vector3D& point,
                    const Vector3D& sphPosition,
                    float radius)
{
  return static_cast<bool>(point.distance(sphPosition) < radius);
}

bool
frustrumContainsPlane(const std::array<Plane, 6>& frustrumPlanes,
                      const Vector3D& planeNormal,
                      const Vector3D& planePoint)
{
  return frustrumPlaneIntersect(frustrumPlanes, planeNormal, planePoint);
}

bool
frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                         const std::array<Plane, 6>& frustrumBPlanes)
{
  return false;
}

bool
frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
                       const Vector3D& sphereOrigin,
                       float sphereRadius)
{
  return false;
}

}
