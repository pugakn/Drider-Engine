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
                        float capsule1Radius,
                        const Vector3D& capsule2A,
                        const Vector3D& capsule2B,
                        float capsule2Radius)
{
 // Compute (squared) distance between the inner structures of the capsules
 float s, t;
 Vector3D c1, c2;
 float dist2 = closestPtSegmentSegment(capsule1A, 
                                       capsule1B,
                                       capsule2A, 
                                       capsule2B, 
                                       s, 
                                       t, 
                                       c1, 
                                       c2);
 float radius = capsule1Radius + capsule2Radius;
 return dist2 <= radius * radius;
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
aabbAabbIntersect(const Vector3D& aabbCenter,
                  float aabbWidth,
                  float aabbHeight,
                  const Vector3D& aabbCenter2,
                  float aabbWidth2,
                  float aabbheight2)
{
  return abs(aabbCenter.x - aabbCenter2.x) <= 
		     ((aabbWidth / 2) + (aabbWidth2 / 2)) &&
		     abs(aabbCenter.y - aabbCenter2.y) <= 
	     	 ((aabbHeight / 2) + (aabbheight2 / 2)) && 
		     abs(aabbCenter.z - aabbCenter2.z) <= 
		     ((aabbWidth / 2) + (aabbWidth2 / 2));
}

bool
aabbSphereIntersect(const Vector3D& aabbCenter,
                    float aabbWidth,
                    float aabbHeight,
                    const Vector3D& sphereOrigin,
                    float sphereRadius)
{
	Vector3D distance = sphereOrigin - aabbCenter;
	float trueDistance = distance.length();
	distance.normalize();
	if (distance.x >= distance.y && distance.x >= distance.z) {
		distance /= distance.x;
	}
	else if (distance.y >= distance.x && distance.y >= distance.z) {
		distance /= distance.y;
	}
	else {
		distance /= distance.z;
	}
	distance.x *= aabbWidth / 2;
	distance.y *= aabbHeight / 2;
	distance.z *= aabbWidth / 2;
	if (trueDistance <= (sphereRadius + distance.length())) {
		return true;
	}
	return false;
}

bool
aabbPlaneIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbHeight,
                   const Vector3D& planeNormal,
                   const Vector3D& planePoint)
{

  return false;
}

bool
aabbFrustrumIntersect(const Vector3D& aabbCenter,
                      float aabbWidth,
                      float aabbHeight,
                      const std::array<Plane, 6>& frustrumPlanes)
{

  return false;
}

bool
aabbRayIntersect(const Vector3D& aabbCenter,
                 float aabbWidth,
                 float aabbHeight,
                 const Vector3D& rayOrigin,
                 const Vector3D& rayDirection)
{
	Vector3D invertedDirection(1.0f / rayDirection.x, 
														 1.0f / rayDirection.y, 
														 1.0f / rayDirection.z);
	Vector3D aabbMax(aabbCenter.x + (aabbWidth / 2),
									 aabbCenter.y + (aabbHeight / 2),
									 aabbCenter.z + (aabbWidth / 2));
	Vector3D aabbMin(aabbCenter.x - (aabbWidth / 2),
									 aabbCenter.y - (aabbHeight / 2),
									 aabbCenter.z - (aabbWidth / 2));
	float tmin = 0.0f, tmax = 0.0f, t1 = 0.0f, t2 = 0.0f, i;
	t1 = (aabbMin.x - rayOrigin.x) * invertedDirection.x;
	t2 = (aabbMax.x - rayOrigin.x) * invertedDirection.x;
	tmin = std::min(t1, t2);
	tmax = std::max(t1, t2);
	for (i = 1; i < 3; ++i) {
		t1 = (aabbMin[i] - rayOrigin[i]) * invertedDirection[i];
		t2 = (aabbMax[i] - rayOrigin[i]) * invertedDirection[i];
		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));
	}
	return tmax > std::max(tmin, 0.0f);
}

bool
aabbPointIntersect(const Vector3D& aabbCenter,
                   float aabbWidth,
                   float aabbHeight,
                   const Vector3D& point)
{
	return (aabbCenter.x - (aabbWidth / 2)) <= point.x &&
		     (aabbCenter.x + (aabbWidth / 2)) >= point.x &&
	       (aabbCenter.y - (aabbHeight / 2)) <= point.y &&
		     (aabbCenter.y + (aabbHeight / 2)) >= point.y &&
         (aabbCenter.z - (aabbWidth / 2)) <= point.z &&
		     (aabbCenter.z + (aabbWidth / 2)) >= point.z;
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


float 
closestPtSegmentSegment(const Vector3D& p1,
                        const Vector3D& q1,
                        const Vector3D& p2,
                        const Vector3D& q2,
                        float &s,
                        float &t,
                        Vector3D& c1,
                        Vector3D& c2)
{
 Vector3D d1 = q1 - p1; // Direction vector of segment S1
 Vector3D d2 = q2 - p2; // Direction vector of segment S2
 Vector3D r = p1 - p2;
 float a = d1.dot(d1); // Squared length of segment S1, always nonnegative
 float e = d2.dot( d2); // Squared length of segment S2, always nonnegative
 float f = d2.dot(r);

 // Check if either or both segments degenerate into points
 if (a <= Math::EPSILON && e <= Math::EPSILON) {
  // Both segments degenerate into points
  s = t = 0.0f;
  c1 = p1;
  c2 = p2;
  return (c1 - c2).dot(c1 - c2);
 }
 if (a <= Math::EPSILON) {
  // First segment degenerates into a point
  s = 0.0f;
  t = f / e; // s = 0 => t = (b*s + f) / e = f / e
  t = Math::clamp(t, 0.0f, 1.0f);
 } else 
 {
  float c = d1.dot( r);
  if (e <= Math::EPSILON) {
   // Second segment degenerates into a point
   t = 0.0f;
   s = Math::clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
 }
 else {
  // The general nondegenerate case starts here
  float b = d1.dot(d2);
  float denom = a*e - b*b; // Always nonnegative
                               // If segments not parallel, compute closest point on L1 to L2 and
                               // clamp to segment S1. Else pick arbitrary s (here 0)
  if (denom != 0.0f) {
  s = Math::clamp((b*f - c*e) / denom, 0.0f, 1.0f);
  }
  else {
   s = 0.0f;
  }
   // Compute point on L2 closest to S1(s) using
   // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
   t = (b*s + f) / e;
   // If t in [0,1] done. Else clamp t, recompute s for the new value
   // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
   // and clamp s to [0, 1]
   if (t < 0.0f) {
    t = 0.0f;
    s = Math::clamp(-c / a, 0.0f, 1.0f);
   }
   else if (t > 1.0f) {
    t = 1.0f;
    s = Math::clamp((b - c) / a, 0.0f, 1.0f);
   }
  }
 }
 c1 = p1 + d1 * s;
 c2 = p2 + d2 * t;
 return (c1 - c2).dot(c1 - c2);
}
}
