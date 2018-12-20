#include "dr_serializable_light.h"

#include "dr_file.h"
#include "dr_gameObject.h"
#include "dr_light_component.h"

namespace driderSDK {
void
sLight::load(File &file,
             std::shared_ptr<GameObject> obj) {

  String name;
  file.m_file >> name;

  auto component = obj->createComponent<LightComponent>();
  //component->setName(StringUtils::toTString(name));
  
  Vector3D color;
  file.m_file >> color.x;
  file.m_file >> color.y;
  file.m_file >> color.z;
  component->SetColor(color);
  
  float range;
  file.m_file >> range;
  component->SetRange(range);
  
  float intensity;
  file.m_file >> intensity;
  component->SetIntensity(intensity);

}
}