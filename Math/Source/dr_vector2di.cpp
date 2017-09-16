#include <dr_vector2di.h>

namespace driderSDK {

namespace math {

Vector2DI::Vector2DI() : x(0), y(0) {}

Vector2DI::Vector2DI(const Vector2DI & V) : x(V.x), y(V.y) {}

Vector2DI::Vector2DI(Int32 _x, Int32 _y) : x(_x), y(_y) {}

Vector2DI::~Vector2DI() {}

}

}