#include <dr_vector2di.h>

namespace driderSDK {

namespace math {

Vector2DI::Vector2DI() : x(0), y(0) {}

Vector2DI::Vector2DI(const Vector2DI & V) : x(V.x), y(V.y) {}

Vector2DI::Vector2DI(__int32 _x, __int32 _y) : x(_x), y(_y) {}

Vector2DI::~Vector2DI() {}

}

}