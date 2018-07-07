#pragma once

#include <vector>

#include <dr_vector3d.h>

namespace driderSDK {

class PathHolder
{
public:

  void
  pushPoint(const Vector3D& point);

  void 
  popPoint();

  SizeT
  numPoints() const;

  const std::vector<Vector3D>&
  getPoints() const;

  Vector3D
  getPoint(UInt32 index) const;
private:

  std::vector<Vector3D> m_points;
};

}
