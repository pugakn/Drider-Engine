#include <dr_vector4d.h>

namespace driderSDK {

namespace math {

Vector4D::Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vector4D::Vector4D(const Vector4D & V) : x(V.x), y(V.y), z(V.z), w(V.w) {}

Vector4D::Vector4D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

Vector4D::~Vector4D() {}

}

}