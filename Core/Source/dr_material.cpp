#include "dr_material.h"
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_texture_core.h>
#include <dr_texture.h>
#include <dr_string_utils.h>

#include <dr_file.h>

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

Material::Material(const TString& name) : m_name(name) {
}

Material::Material(const Material& other) {
  *this = other;
}

Material& 
Material::operator=(const Material& other) {
  
  for (auto& prop : other.m_properties) {
   
    PropertyPtr newProp;

    switch (prop->type) {
   
    case PROPERTY_TYPE::kFloat:
    newProp = copyProperty<FloatProperty>(prop);
    break;
    case PROPERTY_TYPE::kVec2:
    newProp = copyProperty<Vec2Property>(prop);
    break;
    case PROPERTY_TYPE::kVec3:
    newProp = copyProperty<Vec3Property>(prop);
    break;
    case PROPERTY_TYPE::kVec4:
    newProp = copyProperty<Vec4Property>(prop);
    break;
    default:
    break;
    }

    DR_ASSERT(newProp);

    m_properties.push_back(std::move(newProp));
  }
    
  return *this;
}

void 
Material::set() {
  for (SizeT i = 0; i < m_properties.size(); ++i) {
    if (auto t = m_properties[i]->texture.lock()) {
      t->textureGFX->set(GraphicsAPI::getDeviceContext(), i);
    }
  }
}

void
Material::setTexture(std::shared_ptr<TextureCore> texture, 
                     const TString& propertyName) {
  if (auto prop = getProperty(propertyName)) {
    prop->texture = texture;
  }
}

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

bool
Material::projectShadow() const {
  return m_proyectShadow;
}

void
Material::serialize(File &file) {
  file.m_file << StringUtils::toString(m_name) << "\n";
  m_proyectShadow = true;
  file.m_file << m_proyectShadow << "\n";

  file.m_file << m_properties.size() << "\n";

  for(auto &prop : m_properties) {
    file.m_file << StringUtils::toString(prop->name) << "\n";
    file.m_file << (UInt32)prop->type << "\n";
    auto tex = prop->texture.lock();
    if(tex) {
      String textName = StringUtils::toString(tex->getName());
      file.m_file << textName << "\n";
    }
    else
      file.m_file << "DUMMY_TEXTURE\n";
  }

  /*
  TString name;
  const PROPERTY_TYPE::E type;
  std::weak_ptr<TextureCore> texture;
  */
  
  
}

void Material::setProyectShadow(bool bShadow) {
  m_proyectShadow = bShadow;
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

TString Material::interpretType(PROPERTY_TYPE::E& _type, Property* _prop) {
  /*TString val = L"";
  if(_type == PROPERTY_TYPE::E::kFloat) {
    FloatProperty* prop = static_cast<FloatProperty*>(_prop);
    val += prop->value;
    val += L"\n";
    val += prop->readChannel;
  }
  else if (_type == PROPERTY_TYPE::E::kVec2) {
    Vec2Property* prop = static_cast<Vec2Property*>(_prop);
    Vector2D vec = prop->value;
    val += StringUtils::toTString(vec.x) + L"\n" +
           StringUtils::toTString(vec.y);
    val += L"\n";
    val += prop->readChannels[0];
    val += prop->readChannels[1];
  }
  else if (_type == PROPERTY_TYPE::E::kVec3) {
    Vec3Property* prop = static_cast<Vec3Property*>(_prop);
    Vector3D vec = prop->value;
    val += StringUtils::toTString(vec.x) + L"\n" +
           StringUtils::toTString(vec.y) + L"\n" +
           StringUtils::toTString(vec.z);
    val += L"\n";
    val += prop->readChannels[0];
    val += prop->readChannels[1];
    val += prop->readChannels[2];
  }
  else if (_type == PROPERTY_TYPE::E::kVec4) {
    Vec4Property* prop = static_cast<Vec4Property*>(_prop);
    Vector4D vec = prop->value;
    val += StringUtils::toTString(vec.x) + L"\n" +
           StringUtils::toTString(vec.y) + L"\n" +
           StringUtils::toTString(vec.z) + L"\n" +
           StringUtils::toTString(vec.w);
    val += L"\n";
    val += prop->readChannels[0];
    val += prop->readChannels[1];
    val += prop->readChannels[2];
    val += prop->readChannels[3];
  }*/
  return L"";
}

}