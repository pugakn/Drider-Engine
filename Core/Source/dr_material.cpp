#include "dr_material.h"

namespace driderSDK {
Material::FloatProperty* Material::addProperty(const TString & name, float value, CHANNEL readChannel) {
  return nullptr;
}

Material::Vec2Property* Material::addProperty(const TString & name, const Vector2D & value, const std::array<CHANNEL, 2>& readChannels) {
  return nullptr;
}

Material::Vec3Property* Material::addProperty(const TString & name, const Vector3D & value, const std::array<CHANNEL, 3>& readChannels) {
  return nullptr;
}

Material::Vec4Property* Material::addProperty(const TString & name, const Vector4D & value, const std::array<CHANNEL, 4>& readChannels) {
  return nullptr;
}

Material::FloatProperty* Material::getFloatProperty(const TString & name) {
  return nullptr;
}

Material::Vec2Property* Material::getVec2Property(const TString & name) {
  return nullptr;
}

Material::Vec3Property* Material::getVec3Property(const TString & name) {
  return nullptr;
}

Material::Vec4Property* Material::getVec4Property(const TString & name) {
  return nullptr;
}

void Material::removeProperty(const TString & name) {}
SizeT Material::getPropertiesCount() {
  return SizeT();
}

Property* Material::getProperty(SizeT index) {
  return nullptr;
}

Property* Material::getProperty(const TString & name) {
  return nullptr;
}

Property* Material::transformProperty(const TString & name, PROPERTY_TYPE newType) {
  return nullptr;
}
}