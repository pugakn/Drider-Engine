#pragma once
#include "dr_Pixel.h"
#include <dr_vector2d.h>
#include <dr_vector3d.h>

namespace driderSDK {

struct TextureCompressor
{

static Vector3D
color2Vector3D(Pixel color);

static Pixel
Vector3D2Color(Vector3D vec);

static Vector3D
Spherical2Cartesian(Vector2D direction);

static Vector2D
Cartesian2Spherical(Vector3D direction);

static float
int2float(UInt8& number);

static UInt8
float2int(float& number);

};

}