#include "dr_directional_light_component.h"

#include <dr_id_object.h>
#include <dr_gameObject.h>
#include <dr_graph.h>

namespace driderSDK {

DirectionalLightComponent::DirectionalLightComponent(GameObject& _gameObject) 
  : GameComponent(_gameObject, _T("DirectionalLightComponent")),
    m_vec3Color(Vector3D(1.0f, 1.0f, 1.0f)),
    m_bCastShadow(true),
    m_fIntensity(1.0f) {
}

void
DirectionalLightComponent::SetColor(const Vector3D& newColor) {
  m_vec3Color = newColor;
}

void
DirectionalLightComponent::SetCastShadow(const bool newCastShadow) {
  m_bCastShadow = newCastShadow;
}

void
DirectionalLightComponent::SetIntensity(const float newIntensity) {
  m_fIntensity = newIntensity;
}

Vector4D
DirectionalLightComponent::GetDirectionShadow() {
  Vector4D DirectionShadow = m_gameObject.getTransform().getDirection();
  DirectionShadow.w = static_cast<float>(m_bCastShadow);
  return DirectionShadow;
}

Vector4D
DirectionalLightComponent::GetColorIntensity() {
  return Vector4D(m_vec3Color, m_fIntensity);
}

Vector3D
DirectionalLightComponent::GetColor() {
  return m_vec3Color;
}

bool
DirectionalLightComponent::GetCastShadow() {
  return m_bCastShadow;
}

float
DirectionalLightComponent::GetIntensity() {
  return m_fIntensity;
}

void
DirectionalLightComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Light << "\n";
  String name = StringUtils::toString(getName());
  file.m_file << name << "\n";

  file.m_file << m_vec3Color.x << "\n";
  file.m_file << m_vec3Color.y << "\n";
  file.m_file << m_vec3Color.z << "\n";

  file.m_file << m_bCastShadow << "\n";

  file.m_file << m_fIntensity << "\n";
  
}

void
DirectionalLightComponent::deserialize(TString &data) {

}

void
DirectionalLightComponent::onCreate() {
  SceneGraph::instance().addLight(this);
}

void 
DirectionalLightComponent::onUpdate() {
}

void 
DirectionalLightComponent::onRender() {
}

void 
DirectionalLightComponent::onDestroy() {
  SceneGraph::instance().removeLight(this);
}

UInt32
DirectionalLightComponent::getClassID() {
  return CLASS_NAME_ID(DirectionalLightComponent);
}

GameComponent* 
DirectionalLightComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<DirectionalLightComponent>();
}

}