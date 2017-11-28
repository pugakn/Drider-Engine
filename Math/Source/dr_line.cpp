#include "dr_line.h"

namespace driderSDK {

  void
  Line::Set(Vector3D _origin, Vector3D _direction, float _length)
  {
    pointOrigin = _origin;
    length = _length;
    pointEnd = pointOrigin + _direction.normalize() * length;
  }

  void
  Line::Set(Vector3D _origin, Vector3D _end)
  {
    pointOrigin = _origin;
    pointEnd = _end;
    length = pointOrigin.distance(pointEnd);
  }

  Vector3D
  Line::GetPoint(SizeT point)
  {
    DR_ASSERT(point < 2);
    if (point)
    {
      return pointEnd;
    }
    return pointOrigin;
  }

  float
  Line::Length()
  {
    return length;
  }

  bool
  Line::intersects(const Plane & plane) const
  {
    return false;
  }
  
  bool
  Line::intersects(const Sphere & sphere) const
  {
    return false;
  }

  bool
  Line::intersects(const Capsule & capsule) const
  {
    return false;
  }

  bool
  Line::intersects(const Frustrum & frustrum) const
  {
    return false;
  }
}