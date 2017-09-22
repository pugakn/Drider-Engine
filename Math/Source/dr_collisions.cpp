#include "dr_collisions.h"

#include "dr_math.h"
#include "dr_sphere.h"
#include "dr_capsule.h"
#include "dr_ray.h"
#include "dr_vector3d.h"


namespace driderSDK {

//Uncommented
bool PointInSphere(
  const Vector3D& vec,
  const Sphere& s1)
{
  return s1.getCenter().distance(vec) < s1.getRadio();
}
//Uncommented
bool CollSphereSphere(
  const Sphere& s1, 
  const Sphere& s2)
{
 return s1.getCenter().distance(s2.getCenter()) <
        (s1.getRadio() + s2.getRadio());
}

//Uncommented
bool CollSphereCapsule(
  const Sphere& s1,
  const Capsule& c1)
{
  return false;
}
//Uncommented
bool CollSphereRay(
  const Sphere& s1,
  const Ray& r1)
{
  if(PointInSphere(r1.getOrigin(), s1))
    return true;
  Vector3D vec(r1.getOrigin() - s1.getCenter());

  float pDirection = vec.dot(r1.getDirection());

  if(pDirection < 0.0f)
    return false;

  Vector3D vecClosestPoint(r1.getOrigin() + (r1.getDirection() * pDirection));

  return PointInSphere(vecClosestPoint, s1);
}

}
