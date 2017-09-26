#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;
class Ray;

class DR_API_EXPORT AABB
{
 public:
  
  /**
  * Default constructor
  *
  */
  AABB();

  /**
  * Initialize the constructor with the given values
  *
  * @param s
  *  Size of the AABB
  *
  * @param C
  *  Center of the box given by a vector 3D
  */
  AABB(Float32 s, const Vector3D& C);

  /**
  * Default destructor
  *
  */
  ~AABB();

  /**
  * Get the size of the AABB
  * 
  * @return
  *  Size of the AABB
  */
  FORCEINLINE Float32
  getSize() {
    return m_fSize;
  }

  /**
  * Get the center of the AABB
  *
  * @return
  *  Center of the AABB
  */
  FORCEINLINE Vector3D
  getCenter() {
    return m_center;
  }

  /**
  * Adjust the size of the AABB
  *
  * @param s
  *  Center of the AABB
  */
  FORCEINLINE void
  setSize(Float32& s) {
    m_fSize = s;
  }

  /**
  * Adjust the center of the AABB
  *
  * @param C
  *  Center of the AABB
  */
  FORCEINLINE void
  setCenter(Vector3D& C) {
    m_center = C;
  }

  /**
  * Checks for an intersection with another AABB
  *
  * @param aabb
  *  The other AABB to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(AABB& aabb);

  /**
  * Checks for an intersection with a sphere
  *
  * @param sphere
  *  The sphere to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(Sphere& sphere);

  /**
  * Checks for an intersection with a plane
  *
  * @param plane
  *  The plane to check
  *
  * @return
  *  True if it intersects
  */
  FORCEINLINE bool
  intersect(Plane& plane);

  /**
  * Checks for an intersection with a frustrum
  *
  * @param frustrum
  *  The frustrum to check
  *
  * @return
  *  True if it intersects
  */
  FORCEINLINE bool
  intersect(Frustrum& frustrum);

  /**
  * Checks for an intersection with a ray
  *
  * @param ray
  *  The ray to check
  *
  * @return
  *  True if the ray intersects with the AABB
  */
  FORCEINLINE bool
  intersect(Ray& ray);

  /**
  * Checks if the point is in the AABB
  *
  * @param point
  *  Vector3D that represents the point
  *
  * @return
  *  True if the point is inside
  */
  FORCEINLINE bool
  intersect(Vector3D& point);
  
 protected:
 private:
  Float32 m_fSize;
  Vector3D m_center;
};

}