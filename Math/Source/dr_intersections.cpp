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
                      float sph2Radius) {
  return static_cast<bool>(sph1Position.distance(sph2Position) <
    (sph1Radius + sph2Radius));
}

bool
sphereCapsuleIntersect(const Vector3D& sphPosition,
                       float sphRadio,
                       const Vector3D& capsuleA,
                       const Vector3D& capsuleB,
                       float capsuleRadius) {
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
                        const Vector3D& capsule2Radius) {
  // I need ClosestPtSegmentSegment, but i don't know where i put it.
  return false;
}
bool
sphereRayIntersect(const Vector3D& sphPosition,
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
rayPlaneIntersect(const Vector3D& rayOrigin,
                  const Vector3D& rayDirection,
                  const Vector3D& planeNormal,
                  const Vector3D& planePoint) {
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
                  float* intersectionPoint) {
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
                     const std::array<Plane, 6>& frustrumPlanes) {
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
                const Vector3D& rayBDirection) {
  return false;
}

bool
frustrumFrustrumIntersect(const std::array<Plane, 6>& frustrumAPlanes,
                          const std::array<Plane, 6>& frustrumBPlanes) {
  return false;
}

bool
frustrumSphereIntersect(const std::array<Plane, 6>& frustrumPlanes,
                        const Vector3D& sphereOrigin,
                        float sphereRadius) {
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
                   const Vector3D& planePoint) {
  //TODO : implement aabb intersection function
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
	float tmin = 0.0f, tmax = 0.0f, t1 = 0.0f, t2 = 0.0f;
	int i = 0;
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

bool
spherePlaneIntersect(const Vector3D& planeNormal,
                     float planeGap,
                     const Vector3D& sphereCenter,
                     float sphereRadius) {
  Plane plane(planeNormal, planeGap);

  return (plane.distanceToPoint(sphereCenter) < sphereRadius);
}

bool aabbPlaneIntersect(const Vector3D& aabbCenter,
                        float aabbWidth,
                        float aabbHeight,
                        const Vector3D& planeNormal,
                        float planeGap) {
  return false;
}

bool capsulePlaneIntersect(const Vector3D& capsuleA,
                           const Vector3D& capsuleB,
                           const Vector3D& capsuleRadius,
                           const Vector3D& planeNormal,
                           float planeGap) {
  return false;
}

bool frustrumPlaneIntersect(const std::array<Plane, 6>& frustrumPlanes,
                            const Vector3D& planeNormal,
                            float planeGap) {

  for (const Plane& plane : frustrumPlanes) {
    if (planePlaneIntersect(plane, plane.d, planeNormal, planeGap)) {
      return true;
    }
  }

  return false;
}

bool planePlaneIntersect(const Vector3D& plane1Normal,
                         float plane1Gap,
                         const Vector3D& plane2Normal,
                         float plane2Gap,
                         Vector3D& point,
                         Vector3D& direction) {

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

bool planePlaneIntersect(const Vector3D& plane1Normal,
                         float plane1Gap,
                         const Vector3D& plane2Normal,
                         float plane2Gap) {

  Vector3D d = plane1Normal.cross(plane2Normal);

  return d.lengthSqr() > Math::EPSILON;

}

/**********************************************************************
*						            Containing functions
**********************************************************************/
bool
sphereContainsPoint(const Vector3D& point,
                    const Vector3D& sphPosition,
                    float radius) {
  return static_cast<bool>(point.distance(sphPosition) < radius);
}

bool
frustrumContainsPlane(const std::array<Plane, 6>& frustrumPlanes,
                      const Vector3D& planeNormal,
                      const Vector3D& planePoint) {
  return frustrumPlaneIntersect(frustrumPlanes, planeNormal, planePoint);
}

bool
frustrumContainsFrustrum(const std::array<Plane, 6>& frustrumAPlanes,
                         const std::array<Plane, 6>& frustrumBPlanes) {
  return false;
}

bool
frustrumContainsSphere(const std::array<Plane, 6>& frustrumPlanes,
                       const Vector3D& sphereOrigin,
                       float sphereRadius) {
  return false;
}

}
