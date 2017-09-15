#include <dr_vector3d.h>

namespace driderSDK {

namespace math {

Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3D::Vector3D(const Vector3D & V) : x(V.x), y(V.y), z(V.z) {}

Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3D::~Vector3D() {}

}

}