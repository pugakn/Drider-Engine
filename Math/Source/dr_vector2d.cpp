#include <dr_vector2d.h>

namespace driderSDK {

namespace math {

Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

Vector2D::Vector2D(const Vector2D & V) : x(V.x), y(V.y) {}

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y) {}

Vector2D::~Vector2D() {}

}

}