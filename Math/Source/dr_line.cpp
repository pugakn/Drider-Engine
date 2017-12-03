#include "dr_line.h"
#include "dr_intersections.h"
#include "dr_ray.h"
#include "dr_plane.h"
#include "dr_sphere.h"

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
  Line::GetPoint(SizeT point) const
  {
    DR_ASSERT(point < 2);
    if (point)
    {
      return pointEnd;
    }
    return pointOrigin;
  }

  float
  Line::Length() const
  {
    return length;
  }

  bool
  Line::intersects(const Plane & plane) const
  {
    Vector3D direction = (pointEnd - pointOrigin).normalize();
    float point;
    if (Intersect::rayPlane(pointOrigin,
                            direction,
                            plane, plane * plane.d,
                            &point)
    )
    {
      Vector3D pointIntersect = pointOrigin + direction * point;
      return (*this).pointInline(pointIntersect);
    }
    return false;
  }
  
  bool
  Line::intersects(const Sphere& sphere) const
  {
    Vector3D pointIntersect;
    if (Intersect::sphereRay(sphere.center,
                             sphere.radius,
                             pointOrigin,
                             pointEnd - pointOrigin,
                             &pointIntersect)
    )
    {
      return (*this).pointInline(pointIntersect);
    }
    return false;
  }

  bool
  Line::intersects(const Capsule& capsule) const
  {
    return false;
  }

  bool
  Line::intersects(const Frustrum& frustrum) const
  {
    return false;
  }
  
  bool
  Line::pointInline(Vector3D& point) const
  {
    Vector3D vectorDirector = pointEnd - pointOrigin;

    float alfa = (point.x - pointOrigin.x) / vectorDirector.x;

    if (point.y == pointOrigin.y + vectorDirector.y * alfa &&
        point.z == pointOrigin.z + vectorDirector.z * alfa
    )
    {
      if (pointOrigin.x > pointEnd.x)
      {
        if (point.x < pointEnd.x || point.x > pointOrigin.x)
        {
          return false;
        }
      }
      else
      {
        if (point.x > pointEnd.x || point.x < pointOrigin.x)
        {
          return false;
        }
      }
    }
    return true;
  }
}