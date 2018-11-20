#include "dr_TextureCompressor.h"

namespace driderSDK {

Vector3D
TextureCompressor::color2Vector3D(Pixel color) {
  return Vector3D(static_cast<float>(color.r),
                  static_cast<float>(color.g),
                  static_cast<float>(color.b));
}

Pixel
TextureCompressor::Vector3D2Color(Vector3D vec) {
  return Pixel(static_cast<UInt8>(vec.x),
               static_cast<UInt8>(vec.y),
                 static_cast<UInt8>(vec.z));
}


Vector3D
driderSDK::TextureCompressor::Spherical2Cartesian(Vector2D direction) {
  Vector3D result;

  const float theta = direction.x;
  const float phi = direction.y;

  result.x = sin(theta) * cos(phi);
  result.y = sin(theta) * sin(phi);
  result.z = cos(theta);

  return result.normalize();
}

Vector2D
TextureCompressor::Cartesian2Spherical(Vector3D direction) {
  Vector2D result;

  // x = theta = acos(z)
  result.x = Math::aCos(direction.z);
  // y = phi = atan2(y, x)
  result.y = Math::aTan2(direction.y, direction.x);

  return result;
}

float
TextureCompressor::int2float(UInt8& number) {
  return *reinterpret_cast<float*>(&number);
}

UInt8
TextureCompressor::float2int(float& number) {
  return *reinterpret_cast<UInt8*>(&number);
}

}