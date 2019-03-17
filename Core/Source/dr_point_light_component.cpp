#include "dr_point_light_component.h"

#include <dr_id_object.h>
#include <dr_gameObject.h>
#include <dr_graph.h>

namespace driderSDK {

PointLightComponent::PointLightComponent(GameObject& _gameObject) 
  : GameComponent(_gameObject, _T("PointLightComponent")),
    m_vec3Color(Vector3D(1.0f, 1.0f, 1.0f)),
    m_fRange(1.0f),
    m_fIntensity(1.0f) {
}

void
PointLightComponent::SetColor(const Vector3D& newColor) {
  m_vec3Color = newColor;
}

void
PointLightComponent::SetRange(const float newRange) {
  m_fRange = newRange;
}

void
PointLightComponent::SetIntensity(const float newIntensity) {
  m_fIntensity = newIntensity;
}

Vector4D
PointLightComponent::GetPositionRange() {
  Vector3D worldPos = m_gameObject.getWorldTransform().getPosition();
  return Vector4D(worldPos, m_fRange);
}

Vector4D
PointLightComponent::GetColorIntensity() {
  return Vector4D(m_vec3Color, m_fIntensity);
}

Vector3D
PointLightComponent::GetColor() {
  return m_vec3Color;
}

float
PointLightComponent::GetRange() {
  return m_fRange;
}

float
PointLightComponent::GetIntensity() {
  return m_fIntensity;
}

void
PointLightComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Light << "\n";
  String name = StringUtils::toString(getName());
  file.m_file << name << "\n";

  file.m_file << m_vec3Color.x << "\n";
  file.m_file << m_vec3Color.y << "\n";
  file.m_file << m_vec3Color.z << "\n";

  file.m_file << m_fRange << "\n";

  file.m_file << m_fIntensity << "\n";
  
}

void
PointLightComponent::deserialize(TString &data) {

}

void
PointLightComponent::onCreate() {
  SceneGraph::instance().addLight(this);
}

void 
PointLightComponent::onUpdate() {
}

void 
PointLightComponent::onRender() {
}

void 
PointLightComponent::onDestroy() {
  SceneGraph::instance().removeLight(this);
}

UInt32
PointLightComponent::getClassID() {
  return CLASS_NAME_ID(PointLightComponent);
}

GameComponent* 
PointLightComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<PointLightComponent>();
}

}