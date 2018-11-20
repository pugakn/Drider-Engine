#include "PathHolder.h"

namespace driderSDK {

void 
PathHolder::pushPoint(const Vector3D& point) {
  m_points.push_back(point);
}

void 
PathHolder::popPoint() {
  m_points.pop_back();
}

SizeT
PathHolder::numPoints() const {
  return m_points.size();
}

const std::vector<Vector3D>& 
PathHolder::getPoints() const {
  return m_points;
}

Vector3D 
PathHolder::getPoint(UInt32 index) const {
  DR_ASSERT(index < m_points.size());
  return m_points[index];
}

}