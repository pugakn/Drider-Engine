#include "dr_material.h"

namespace driderSDK {

//void 
//Material::removeProperty(Property* property) {
//  for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
//    if(it->get() == property) {
//      m_properties.erase(it);
//      break;
//    }
//  }
//}

Material::Property* Material::addProperty(const TString& name, 
                                          PROPERTY_TYPE type) {
  m_properties.push_back(createProperty(name, type));
  
  return m_properties.back().get();
}

void
Material::removeProperty(const TString& name) {
  for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
    if ((*it)->name == name) {
      m_properties.erase(it);
      break;
    }
  }
}

void 
Material::removeProperty(SizeT index) {
  if (index < m_properties.size()) {
    m_properties.erase(m_properties.begin() + index);
  } 
}

SizeT Material::getPropertiesCount() {
  return m_properties.size();
}

Material::Property* 
Material::getProperty(SizeT index) {
  Property* property = nullptr;

  if (index  < m_properties.size()) {
    property = m_properties[index].get();
  }

  return property;
}

Material::Property* 
Material::getProperty(const TString& name) {
  Property* prop = nullptr;

  for (auto& property : m_properties) {
    if (property->name == name) {
      prop = property.get();
      break;
    }
  }

  return prop;
}

Material::Property* 
Material::transformProperty(const TString& name, PROPERTY_TYPE newType) {
  
  Property* transformed = nullptr;

  for (auto& property : m_properties) {
    if (property->name == name) {
      if (property->type != newType) {
        property = createProperty(name, newType);
      }
      transformed = property.get();
      break;
    }
  }

  return transformed;
}

Material::Property*
Material::transformProperty(SizeT index, PROPERTY_TYPE newType) {
  
  Property* transformed = nullptr;

  if (index < m_properties.size()) {
    if (m_properties[index]->type != newType) {
      m_properties[index] = createProperty(m_properties[index]->name, newType);
    }
    transformed = m_properties[index].get();
  }
  
  return transformed;
}

Material::PropertyPtr 
Material::createProperty(const TString& name, PROPERTY_TYPE type) {

  PropertyPtr property;
  
  switch (type) {
  case driderSDK::Material::kFloat:
    property = dr_make_unique<FloatProperty>(name, 0.0f, kA);
  break;
  case driderSDK::Material::kVec2:
    property = dr_make_unique<Vec2Property>(name, 
                                            Vector2D{0.0f, 0.0f}, 
                                            std::array<CHANNEL,2>{kR,kG});
  break;
  case driderSDK::Material::kVec3:
    property = dr_make_unique<Vec3Property>(name, 
                                            Vector3D{0.0f, 0.0f, 0.0f}, 
                                            std::array<CHANNEL,3>{kR,kG, kB});
  break;
  case driderSDK::Material::kVec4:
    property = dr_make_unique<Vec4Property>(name, 
                                            Vector4D{0.0f, 0.0f, 0.0f, 0.0f}, 
                                            std::array<CHANNEL,4>{kR,kG, kB, kA});
  break;
  default:
  break;
  }

  return std::move(property);
}

}