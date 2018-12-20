#include "dr_light_component.h"

#include <dr_id_object.h>
#include <dr_gameObject.h>
#include <dr_graph.h>

namespace driderSDK {

LightComponent::LightComponent(GameObject& _gameObject) 
  : GameComponent(_gameObject, _T("LightComponent")),
    m_vec3Color(Vector3D(1.0f, 1.0f, 1.0f)),
    m_fRange(1.0f),
    m_fIntensity(1.0f) {
  SceneGraph::instance().registerLight(this);
}

void
LightComponent::SetColor(const Vector3D& newColor) {
  m_vec3Color = newColor;
}

void
LightComponent::SetRange(const float newRange) {
  m_fRange = newRange;
}

void
LightComponent::SetIntensity(const float newIntensity) {
  m_fIntensity = newIntensity;
}

Vector4D
LightComponent::GetPositionRange() {
  Vector3D lightPos;
  Vector3D worldPos = m_gameObject.getWorldTransform().getPosition();
  lightPos.x = worldPos.x;
  lightPos.y = worldPos.y;
  lightPos.z = worldPos.z;
  return Vector4D(lightPos, m_fRange);
}

Vector4D
LightComponent::GetColorIntensity() {
  return Vector4D(m_vec3Color, m_fIntensity);
}

Vector3D
LightComponent::GetColor() {
  return m_vec3Color;
}

float
LightComponent::GetRange() {
  return m_fRange;
}

float
LightComponent::GetIntensity() {
  return m_fIntensity;
}

void
LightComponent::serialize(File &file) {
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
LightComponent::deserialize(TString &data) {

}

void
LightComponent::onCreate() {
}

void 
LightComponent::onUpdate() {
}

void 
LightComponent::onRender() {
}

void 
LightComponent::onDestroy() {
}

UInt32
LightComponent::getClassID() {
  return CLASS_NAME_ID(LightComponent);
}

GameComponent* 
LightComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<LightComponent>();
}

}