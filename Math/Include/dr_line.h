#pragma once

#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;

/**
*  iane class with ywo points or direction and size
*
* Sample usage:
*	Line(Vector3D(0,0,0), Vector3D(1,0.5,0.9));
*/
class DR_MATH_EXPORT Line
{
 public:
  /**
  * TEST::defaultConstructor
  * Default constructor.
  *
  */
  Line()
  {}

  /**
  * TEST::origingDirectionLengthCons
  * Constructor using origin and direction
  *
  * @param _origin
  *	 The origin of the line.
  *
  * @param _direction
  *  The direction of the line.
  *
  * @param _length
  *  Size Line
  */
  Line(Vector3D _origin, Vector3D _direction, float _length)
    : pointOrigin(_origin),
      length(_length)
  {
    pointEnd = pointOrigin + _direction.normalize() * length;
  }

  /**
  * TEST::origingEndCons
  * Constructor using origin and direction
  *
  * @param _origin
  *	 The origin of the line.
  *
  * @param _end
  *  End the line.
  *
  */
  Line(Vector3D _origin, Vector3D _end)
    : pointOrigin(_origin),
      pointEnd(_end)
  {
    length = pointOrigin.distance(pointEnd);
  }

  /**
  * TEST::defaultDestructor
  * Destructor
  *
  */
  ~Line()
  {}

  /**
  * TEST::SetOrigingDirectionLength
  * Set params of line
  *
    * @param _origin
  *	 The origin of the line.
  *
  * @param _direction
  *  The direction of the line.
  *
  * @param _length
  *  Size Line
  *
  */
  void
  Set(Vector3D _origin, Vector3D _direction, float _length);
    
  /**
  * TEST::SetOrigingEnd
  * Check if the line intersects with a plane
  *
  * @param _origin
  *	 The origin of the line.
  *
  * @param _end
  *  End the line.
  *
  */
  void
  Set(Vector3D _origin, Vector3D _end);

  /**
  * TEST::GetPoint
  * return point
  *
  * @param point
  *	 point to return 0-1.
  *
  * @return
  * point 0 or 1
  *
  */
  Vector3D
  GetPoint(const SizeT point) const;

  /**
  * TEST::Length
  * return length
  *
  * @return
  * length for line
  *
  */
  float Length() const;
    
  /**
  * TEST::intersectPlane
  * Check if the line intersects with a plane
  *
  *	@param plane
  *	 The plane to check the intersecton.
  *
  * @return
  *	  True if the line intersects with the plane
  *
  */
  bool
  intersects(const Plane& plane) const;

  /**
  * Check if the line intersects with a sphere
  *
  *	@param sphere
  *	 The sphere to check the intersecton.
  *
  * @return
  *	  True if the line intersects with the sphere
  *
  */
  bool
  intersects(const Sphere& sphere) const;

  /**
  * Check if the line intersects with a capsule
  *
  *	@param capsule
  *	 The capsule to check the intersecton.
  *
  * @return
  *	  True if the line intersects with the capsule
  *
  */
  bool
  intersects(const Capsule& capsule) const;

  /**
  * Check if the line intersects with a frustrum
  *
  *	@param frustrum
  *	 The frustrum to check the intersecton.
  *
  * @return
  *	  True if the line intersects with the frustrum
  *
  */
  bool
  intersects(const Frustrum& frustrum) const;

  /**
  * Check if the point exist in line
  *
  *	@param point
  *	 point to check
  *
  * @return
  *	  True if the point is in line
  *
  */
  bool pointInline(Vector3D& point) const;

 private:
  Vector3D pointOrigin;
  Vector3D pointEnd;
  float length;
};


}