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

Property*
Material::addProperty(const TString& name, PROPERTY_TYPE::E type) {

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

Property* 
Material::getProperty(SizeT index) {
  Property* property = nullptr;

  if (index  < m_properties.size()) {
    property = m_properties[index].get();
  }

  return property;
}

Property* 
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

Property* 
Material::transformProperty(const TString& name, PROPERTY_TYPE::E newType) {
  
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

Property*
Material::transformProperty(SizeT index, PROPERTY_TYPE::E newType) {
  
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
Material::createProperty(const TString& name, PROPERTY_TYPE::E type) {

  PropertyPtr property;
  
  switch (type) {
  case PROPERTY_TYPE::kFloat:
    property = dr_make_unique<FloatProperty>(name, 0.0f, CHANNEL::kA);
  break;
  case PROPERTY_TYPE::kVec2:
    property = dr_make_unique<Vec2Property>(name, 
                                            Vector2D{0.0f, 0.0f}, 
                                            std::array<CHANNEL::E,2>{CHANNEL::kR,
                                                                     CHANNEL::kG
                                                                    }
                                           );
  break;
  case PROPERTY_TYPE::kVec3:
    property = dr_make_unique<Vec3Property>(name, 
                                            Vector3D{0.0f, 0.0f, 0.0f}, 
                                            std::array<CHANNEL::E,3>{CHANNEL::kR,
                                                                     CHANNEL::kG, 
                                                                     CHANNEL::kB});
  break;
  case PROPERTY_TYPE::kVec4:
    property = dr_make_unique<Vec4Property>(name, 
                                            Vector4D{0.0f, 0.0f, 0.0f, 0.0f}, 
                                            std::array<CHANNEL::E,4>{CHANNEL::kR,
                                                                     CHANNEL::kG, 
                                                                     CHANNEL::kB, 
                                                                     CHANNEL::kA});
  break;
  default:
  break;
  }

  return std::move(property);
}

}