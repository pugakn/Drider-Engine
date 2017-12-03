#include "dr_intersections.h"
#include "dr_math.h"
#include "dr_vector3d.h"
#include "dr_plane.h"
#include "dr_matrix3x3.h"
#include "dr_line.h"
#include "dr_ray.h"
namespace driderSDK {
/**********************************************************************
*						            Intersection functions
**********************************************************************/

bool
Intersect::sphereSphere(const Vector3D& sph1Position,
                        float sph1Radius,
                        const Vector3D& sph2Position,
                        float sph2Radius) {
  return sph1Position.distance(sph2Position) < sph1Radius + sph2Radius;
}

bool
Intersect::sphereCapsule(const Vector3D& sphPosition,
                         float sphRadio,
                         const Vector3D& capsuleA,
                         const Vector3D& capsuleB,
                         float capsuleRadius) {
  float dist = sphPosition.sqrDistSegment(capsuleA, capsuleB);

  float radius = sphRadio + capsuleRadius;
  return dist <= radius * radius;
}

bool
Intersect::capsuleCapsule(const Vector3D& capsule1A,
                          const Vector3D& capsule1B,
                          float capsule1Radius,
                          const Vector3D& capsule2A,
                          const Vector3D& capsule2B,
                          float capsule2Radius) {
  // Compute (squared) distance between the inner structures of the capsules
  float s, t;
  Vector3D c1, c2;
  float dist2 = closestPointSegmentSegment(capsule1A,
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
Intersect::sphereRay(const Vector3D& sphPosition, 
                     float sphRadio, 
                     const Vector3D& rayOrigin, 
                     const Vector3D& rayDirection) {
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
Intersect::sphereRay(const Vector3D& sphPosition,
                     float sphRadio,
                     const Vector3D &rayOrigin,
                     const Vector3D& rayDirection,
                     Vector3D * intersectionPoint) {
  
  if (sphereContainsPoint(rayOrigin, sphPosition, sphRadio)) {
    (*intersectionPoint) = rayOrigin;
    return true;
  }
  Vector3D vec(sphPosition - rayOrigin);

  float pDirection = vec.dot(rayDirection);

  if (pDirection <= 0.0f) {
    return false;
  }

  Vector3D vecClosestPoint(rayOrigin + (rayDirection * pDirection));
  if ((vecClosestPoint - sphPosition).length() == sphRadio)
  {
    (*intersectionPoint) = vecClosestPoint;
    return true;
  }
  if (sphereContainsPoint(vecClosestPoint, sphPosition, sphRadio)) {
    float vecClosestPointLength = vecClosestPoint.length();
    float distanceToPerimeter = Math::sqrt(sphRadio*sphRadio - vecClosestPointLength*vecClosestPointLength);
    (*intersectionPoint) = vecClosestPoint - rayDirection * distanceToPerimeter;
    return true;
  } 
  return false;
}

bool
Intersect::rayRay(const Vector3D& rayOrigin1,
                  const Vector3D& rayDirection1,
                  const Vector3D& rayOrigin2,
                  const Vector3D& rayDirection2,
                  Vector3D* Point) {

  if (rayDirection1.cross(rayDirection2).length() == 0)
  {
    Vector3D directionTemp ((rayOrigin2 - rayOrigin1).normalize());
    if (directionTemp.equals(rayDirection1))
    {
      return true;
      (*Point) = rayOrigin2;
    }
    return false;
  }

  driderSDK::Vector3D flags(0, 0, 0);
  driderSDK::Vector3D R1;
  driderSDK::Vector3D R2;
  driderSDK::Vector3D R3;

  if (rayDirection1.x > 0)
  {
    R1 = driderSDK::Vector3D(rayDirection1.x, -rayDirection2.x, rayOrigin2.x - rayOrigin1.x);
    flags.x = 1;
  }
  else if (rayDirection1.y > 0)
  {
    R1 = driderSDK::Vector3D(rayDirection1.y, -rayDirection2.y, rayOrigin2.y - rayOrigin1.y);
    flags.y = 1;
  }
  else if (rayDirection1.z > 0)
  {
    R1 = driderSDK::Vector3D(rayDirection1.z, -rayDirection2.z, rayOrigin2.z - rayOrigin1.z);
    flags.z = 1;
  }
  else
  {
    return false;
  }

  if (rayDirection2.x > 0 && flags.x == 0)
  {
    R2 = driderSDK::Vector3D(rayDirection1.x, -rayDirection2.x, rayOrigin2.x - rayOrigin1.x);
    flags.x = 1;
  }
  else if (rayDirection2.y > 0 && flags.y == 0)
  {
    R2 = driderSDK::Vector3D (rayDirection1.y, -rayDirection2.y, rayOrigin2.y - rayOrigin1.y);
  }
  else if (rayDirection2.z > 0 && flags.z == 0)
  {
    R2 = driderSDK::Vector3D(rayDirection1.z, -rayDirection2.z, rayOrigin2.z - rayOrigin1.z);
  }
  
  if (flags.x == 0)
  {
    R3 = driderSDK::Vector3D(rayDirection1.x, -rayDirection2.x, rayOrigin2.x - rayOrigin1.x);
  }
  else if (flags.y == 0)
  {
    R3 = driderSDK::Vector3D(rayDirection1.y, -rayDirection2.y, rayOrigin2.y - rayOrigin1.y);
  }
  else if (flags.z == 0)
  {
    R3 = driderSDK::Vector3D(rayDirection1.z, -rayDirection2.z, rayOrigin2.z - rayOrigin1.z);
  }

  R2 = R2 - R1 * (R2.x / R1.x);
  R3 = R3 - R1 * (R3.x / R1.x);
  R3 = R3 - R2 * (R3.y / R2.y);

  if (!R3.equals(driderSDK::Vector3D(0, 0, 0))) {
    return false;
  }
  
  float t = R2.z / R2.y;
  float s = (R1.z - R1.y * t) / R1.x;
  if (s < 0)
  {
    return false;
  }
  (*Point).x = rayOrigin1.x + s * rayDirection1.x;
  (*Point).y = rayOrigin1.y + s * rayDirection1.y;
  (*Point).z = rayOrigin1.z + s * rayDirection1.z;

  return true;
}

bool
Intersect::rayPlane(const Vector3D& rayOrigin,
                    const Vector3D& rayDirection,
                    const Vector3D& planeNormal,
                    float planeGap) {
  Vector3D segment = rayDirection;

	float den = planeNormal.dot(segment);

	if (Math::abs(den) <= Math::EPSILON) {
		return false;
	}

	float dist = (planeNormal.dot(rayOrigin) - planeGap) / den;

	if (dist > Math::EPSILON) {
		return false;
	}

  return true;
}

bool
Intersect::rayPlane(const Vector3D& rayOrigin,
                    const Vector3D& rayDirection,
                    const Vector3D& planeNormal,
                    const Vector3D& planePoint) {
  float denom = planeNormal.dot(rayDirection);
  if (denom > Math::SMALL_NUMBER || denom < -Math::SMALL_NUMBER) {
    Vector3D planeToRayOrigin = planePoint - rayOrigin;
    float t = planeToRayOrigin.dot(planeNormal) / denom;
    return t >= 0;
  }
  return false;
}

bool
Intersect::rayPlane(const Vector3D& rayOrigin,
                    const Vector3D& rayDirection,
                    const Vector3D& planeNormal,
                    const Vector3D& planePoint,
                    float* intersectionPoint) {
  float denom = planeNormal.dot(rayDirection);
  if (denom > Math::SMALL_NUMBER || denom < -Math::SMALL_NUMBER) {
    Vector3D planeToRayOrigin = planePoint - rayOrigin;
    *intersectionPoint = planeToRayOrigin.dot(planeNormal) / denom;
    return static_cast<bool>(*intersectionPoint >= 0);
  }
  return false;
}

bool
Intersect::rayCapsule(const Vector3D& pointSA,
                      const Vector3D& pointSB,
                      const Vector3D& pointP,
                      const Vector3D& pointQ,
                      float r,
                      float& t) {
  Vector3D dir = pointSB - pointSA;
  dir.normalize();
  if (sphereRay(pointP, r, pointSA, dir)) {
    return true;
  }

  dir = pointSB - pointSA;
  if (sphereRay(pointQ, r, pointSA, dir)) {
    return true;
  }

  Vector3D d = pointQ - pointP, m = pointSA - pointP, n = pointSB - pointSA;
  float md = m.dot(d);
  float nd = n.dot(d);
  float dd = d.dot(d);

  if (md < 0.0f && md + nd < 0.0f) {
    return false;
  }
  if (md > dd && md + nd > dd) {
    return false;
  }
  float nn = n.dot(n);
  float mn = m.dot(n);
  float a = dd * nn - nd * nd;
  float k = m.dot(m) - r * r;
  float c = dd * k - md * md;

  if (Math::abs(a) < Math::EPSILON) {

    if (c > 0.0f) {
      return false;
    }

    if (md < 0.0f) {
      t = -mn / nn;
    }
    else if (md > dd) {
      t = (nd - mn) / nn;
    }
    else {
      t = 0.0f;
    }
    return true;

  }

  float b = dd * mn - nd * md;
  float discr = b * b - a * c;
  if (discr < 0.0f) {
    return false;
  }

  t = (-b - Math::sqrt(discr)) / a;
  if (t < 0.0f || t > 1.0f) {
    return false;
  }
  if (md + t * nd < 0.0f) {
    if (nd <= 0.0f) {
      return 0;
    }
    t = -md / nd;
    return k + 2 * t * (mn + t * nn) <= 0.0f;
  }
  else if (md + t * nd > dd) {
    if (nd >= 0.0f) {
      return false;
    }
    t = (dd - md) / nd;
    return k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0f;
  }
  return true;
}

bool
Intersect::rayLine(const Vector3D& rayOrigin,
                   const Vector3D& rayDirection,
                   const Vector3D& point1,
                   const Vector3D& point2,
                   Vector3D* Point)
{
  Line line(point1, point2);
  Ray temp(point1, (point2 - point1).normalize());
  Ray ray(rayOrigin, rayDirection);
  if (ray.intersects(temp, Point))
  {
    return line.pointInline((*Point));
  }
  return false;
}

bool
Intersect::rayFrustrum(const Vector3D& rayOrigin,
                       const Vector3D& rayDirection,
                       const std::array<Plane, 6>& frustrumPlanes) {
  return false;
}

bool
Intersect::frustrumFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                            const std::array<Plane, 6>& frustrumBPlanes) {
  return false;
}

bool
Intersect::frustrumSphere(const std::array<Plane, 6>& frustrumPlanes,
                          const Vector3D& sphereOrigin,
                          float sphereRadius) {
  return false;
}

bool
Intersect::aabbAabb(const Vector3D& aabbCenter,
										float aabbWidth,
										float aabbHeight,
										float aabbDepth,
                    const Vector3D& aabbCenter2,
										float aabbWidth2,
										float aabbHeight2,
										float aabbDepth2) {
  return abs(aabbCenter.x - aabbCenter2.x) <=
    ((aabbWidth * 0.5f) + (aabbWidth2 * 0.5f)) &&
    abs(aabbCenter.y - aabbCenter2.y) <=
    ((aabbHeight * 0.5f) + (aabbHeight2 * 0.5f)) &&
    abs(aabbCenter.z - aabbCenter2.z) <=
    ((aabbDepth * 0.5f) + (aabbDepth2 * 0.5f));
}

float checkPoint(float pn, float bmin, float bmax) {
  float out = 0;
  float v = pn;
  if (v < bmin)
  {
    float val = (bmin - v);
    out += val * val;
  }
  if (v > bmax)
  {
    float val = (v - bmax);
    out += val * val;
  }
  return out;
}

bool
Intersect::aabbSphere(const Vector3D& min,
                      const Vector3D& max,
                      const Vector3D& sphereCenter,
                      float sphereRadius) {

  float sq = 0.0f;
  sq += checkPoint(sphereCenter.x, min.x, max.x);
  sq += checkPoint(sphereCenter.y, min.y, max.y);
  sq += checkPoint(sphereCenter.z, min.z, max.z);
  
  return sq <= (sphereRadius * sphereRadius);
}

bool
Intersect::aabbFrustrum(const Vector3D& aabbCenter,
												float aabbWidth,
												float aabbHeight,
												float aabbDepth,
                        const std::array<Plane, 6>& frustrumPlanes) {

  return false;
}

bool
Intersect::aabbRay(const Vector3D& aabbCenter,
									 const Vector3D& aabbMax,
									 const Vector3D& aabbMin,
                   const Vector3D& rayOrigin,
                   const Vector3D& rayDirection) {
  DR_ASSERT(rayDirection.x != 0.0f);
  DR_ASSERT(rayDirection.y != 0.0f);
  DR_ASSERT(rayDirection.z != 0.0f);
	Vector3D normDirection(rayDirection);
	normDirection.normalize();
  Vector3D invertedDirection(1.0f / normDirection.x,
                             1.0f / normDirection.y,
                             1.0f / normDirection.z);
  float t1 = (aabbMin.x - rayOrigin.x) * invertedDirection.x;
  float t2 = (aabbMax.x - rayOrigin.x) * invertedDirection.x;
  float tmin = Math::min(t1, t2);
  float tmax = Math::max(t1, t2);

  for (Int32 i = 1; i < 3; ++i) {
    t1 = (aabbMin[i] - rayOrigin[i]) * invertedDirection[i];
    t2 = (aabbMax[i] - rayOrigin[i]) * invertedDirection[i];
    tmin = Math::max(tmin, Math::min(t1, t2));
    tmax = Math::min(tmax, Math::max(t1, t2));
  }
  return tmax > Math::max(tmin, 0.0f);
}

bool Intersect::rayAABB(const Vector3D& aabbMax,
                        const Vector3D& aabbMin,
                        const Vector3D& rayOrigin,
                        const Vector3D& rayDirection,
                        Vector3D * point)
{
  enum Side
  {
    RIGHT = 0,
    LEFT,
    MIDDLE
  };
  Vector3D quadrant;
  int i;
  int whichPlane;
  Vector3D maxT;
  Vector3D candidatePlane;
  bool inside = true;

  for (i = 0; i < 3; i++) {
    if (rayOrigin[i] < aabbMin[i]) {
      quadrant[i] = Side::LEFT;
      candidatePlane[i] = aabbMin[i];
      inside = false;
    }
    else if (rayOrigin[i] > aabbMax[i]) {
      quadrant[i] = Side::RIGHT;
      candidatePlane[i] = aabbMax[i];
      inside = false;
    }
    else {
      quadrant[i] = Side::MIDDLE;
    }
  }

  if (inside) {
    (*point) = rayOrigin;
    return true;
  }
  for (i = 0; i < 3; i++) {
    if (quadrant[i] != MIDDLE && rayDirection[i] != 0.)
      maxT[i] = (candidatePlane[i] - rayOrigin[i]) / rayDirection[i];
    else
      maxT[i] = -1.;
  }

  /* Get largest of the maxT's for final choice of intersection */
  whichPlane = 0;
  for (i = 1; i < 3; i++) {
    if (maxT[whichPlane] < maxT[i])
      whichPlane = i;
  }

  /* Check final candidate actually inside box */
  if (maxT[whichPlane] < 0.) {
    return false;
  }
  for (i = 0; i < 3; i++) {
    if (whichPlane != i) {
      (*point)[i] = rayOrigin[i] + maxT[whichPlane] * rayDirection[i];
      if ((*point)[i] < aabbMin[i] || (*point)[i] > aabbMax[i])
        return false;
    }
    else {
      (*point)[i] = candidatePlane[i];
    }
  }
  return true;
}

bool
Intersect::aabbPoint(const Vector3D& aabbCenter,
										 const Vector3D& aabbMax,
										 const Vector3D& aabbMin,
                     const Vector3D& point) {

	return (point.x > aabbMin.x && point.x < aabbMax.x &&
					point.y > aabbMin.y && point.y < aabbMax.y &&
					point.z > aabbMin.z && point.z < aabbMax.z);
}

bool
Intersect::spherePlane(const Vector3D& planeNormal,
                       float planeGap,
                       const Vector3D& sphereCenter,
                       float sphereRadius) {
  return Math::abs(planeNormal.dot(sphereCenter) - planeGap) < sphereRadius;
}

bool
Intersect::aabbPlane(const Vector3D& aabbCenter,
										 float aabbWidth,
										 float aabbHeight,
										 float aabbDepth,
                     const Vector3D& planeNormal,
                     float planeGap) {
  Vector3D extents = aabbCenter;
  extents.x += aabbWidth * 0.5f;
  extents.y += aabbHeight * 0.5f;

  float intervalRad = extents.x*Math::abs(planeNormal.x) +
    extents.y*Math::abs(planeNormal.y) +
    extents.z*Math::abs(planeNormal.z);

  float distance = Math::abs(planeNormal.dot(aabbCenter) + planeGap);

  return distance <= intervalRad;
}

bool
Intersect::segmentPlane(const Vector3D& linePointA,
                        const Vector3D& linePointB,
                        const Vector3D& planeNormal,
                        float planeGap) {
  Vector3D segmentDir = linePointA - linePointB;

  float d = planeNormal.dot(segmentDir);

  if (Math::abs(d) < Math::EPSILON) {
    return false;
  }

  float dist = (planeNormal.dot(linePointA) - planeGap) / d;

  if (dist < -Math::EPSILON || dist > (1.0 + Math::EPSILON)) {
		return false;
	}

  return true;
}

bool
Intersect::capsulePlane(const Vector3D& capsuleA,
                        const Vector3D& capsuleB,
                        float capsuleRadius,
                        const Vector3D& planeNormal,
                        float planeGap) {
  return  spherePlane(planeNormal, planeGap, capsuleA, capsuleRadius) ||
          spherePlane(planeNormal, planeGap, capsuleB, capsuleRadius) ||
          segmentPlane(capsuleA, capsuleB, planeNormal, planeGap);
}

bool
Intersect::frustrumPlane(const std::array<Plane, 6>& frustrumPlanes,
                         const Vector3D& planeNormal) {
  for (const Plane& plane : frustrumPlanes) {
    if (planePlane(plane, planeNormal)) {
      return true;
    }
  }
  return false;
}

bool
Intersect::planePlane(const Vector3D& plane1Normal, 
                      float plane1Gap, 
                      const Vector3D& plane2Normal, 
                      float plane2Gap, 
                      Vector3D& point, Vector3D& direction) {
  //Warning: This assumes that normal1 and normal2 are normalized.

  direction = plane1Normal.cross(plane2Normal);

  float denom = direction.lengthSqr();

  if (denom < Math::EPSILON) {
    return false;
  }

  Vector3D left = plane2Normal*plane1Gap - plane1Normal*plane2Gap;

  point = left.cross(direction) / denom;

  return true;
}

bool
Intersect::planePlane(const Vector3D& plane1Normal,
                      const Vector3D& plane2Normal) {
  Vector3D d = plane1Normal.cross(plane2Normal);

  return d.lengthSqr() > Math::EPSILON;
}

/**********************************************************************
*						            Containing functions
**********************************************************************/

bool
Intersect::sphereContainsPoint(const Vector3D& point,
                               const Vector3D& sphPosition,
                               float radius) {
  return point.distance(sphPosition) <= radius;
}

/*bool
Intersect::frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                                    const std::array<Plane, 6>& frustrumBPlanes) {
  return false;
}

bool
Intersect::frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
                                  const Vector3D& sphereOrigin,
                                  float sphereRadius) {
  return false;
}*/

float Intersect::closestPointSegmentSegment(const Vector3D& p1, 
                                            const Vector3D& q1, 
                                            const Vector3D& p2, 
                                            const Vector3D& q2, 
                                            float& s, 
                                            float& t, 
                                            Vector3D& c1, 
                                            Vector3D& c2) {
  Vector3D d1 = q1 - p1; // Direction vector of segment S1
  Vector3D d2 = q2 - p2; // Direction vector of segment S2
  Vector3D r = p1 - p2;
  float a = d1.dot(d1); // Squared length of segment S1, always nonnegative
  float e = d2.dot(d2); // Squared length of segment S2, always nonnegative
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
  }
  else {
    float c = d1.dot(r);
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