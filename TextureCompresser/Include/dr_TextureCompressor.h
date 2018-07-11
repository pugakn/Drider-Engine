#include "dr_Pixel.h"
#include <dr_vector2d.h>
#include <dr_vector3d.h>

namespace driderSDK {

struct TextureCompressor
{

Vector3D
color2Vector3D(Pixel color);

Pixel
Vector3D2Color(Vector3D vec);

Vector3D
Spherical2Cartesian(Vector2D direction);

Vector2D
Cartesian2Spherical(Vector3D direction);

};

}