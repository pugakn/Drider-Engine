#pragma once

#include <array>
#include <vector>
#include <dr_memory.h>
#include <dr_vector2d.h>
#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

class DR_CORE_EXPORT Material : public Resource
{
 public:
  
  enum CHANNEL
  {
    kR,
    kG,
    kB,
    kA
  };

  enum PROPERTY_TYPE
  {
    kFloat,
    kVec2,
    kVec3,
    kVec4
  };

  struct Property
  {
   public:
    Property(PROPERTY_TYPE type_) 
    : type(type_)
    {}
    TString name;
    const PROPERTY_TYPE type;
    //Texture* texture;
  };

  using PropertyPtr = std::shared_ptr<Property>;
  using PropertyList = std::vector<PropertyPtr>;

  struct FloatProperty : Property
  {
    FloatProperty(float value_, CHANNEL readChannel_)
    : Property(kFloat),
      value(value_),
      readChannel(readChannel_)
    {}

    float value;
    CHANNEL readChannel;
  };

  struct Vec2Property : Property
  {
    Vec2Property(const Vector2D& value_, 
                 const std::array<CHANNEL, 2>& readChannels_) 
    : Property(kVec2),
      value(value_),
      readChannels(readChannels_)
    {
    }

    Vector2D value;
    std::array<CHANNEL, 2> readChannels;
  };

  struct Vec3Property : Property
  {
    Vec3Property(const Vector3D& value_, 
                 const std::array<CHANNEL, 3>& readChannels_) 
    : Property(kVec3),
      value(value_),
      readChannels(readChannels_)
    {
    }

    Vector3D value;
    std::array<CHANNEL, 3> readChannels;  
  };

  struct Vec4Property : Property
  {
    Vec4Property(const Vector4D& value_, 
                 const std::array<CHANNEL, 4>& readChannels_) 
    : Property(kVec3),
      value(value_),
      readChannels(readChannels_)
    {
    }

    Vector4D value;
    std::array<CHANNEL, 4> readChannels;
  };

  /**
  * Gets the default properties for a material 
  * 
  * @return
  *   A list with all the default properties.
  */
  static PropertyList 
  defaultProperties();

  /**
  * Adds a new property of float type to the material properperties.
  * 
  * @param name
  *  The name of the property. If there was another property with that name
  *  in the material properties, it will be overwritten.
  *  
  * @param value
  *  The default value of the property, it will be mixed with the read value 
  *  of the texture if it exists.
  * 
  * @param readChannel
  *  The channel from which the information for the value of the property 
  *  will be taken.
  * 
  * @return
  *   A pointer to the property created. It could return nullptr if there 
  *   is an error while trying to allocate memory.
  */
  FloatProperty*
  addProperty(const TString& name, 
              float value = 0.0f, 
              CHANNEL readChannel = kA);

  /**
  * Adds a new property of Vector2D type to the material properperties.
  *
  * @param name
  *  The name of the property. If there was another property with that name
  *  in the material properties, it will be overwritten.
  *  
  * @param value
  *  The default value of the property, it will be mixed with the read value 
  *  of the texture if it exists.
  * 
  * @param readChannel
  *  The channel from which the information for the value of the property 
  *  will be taken.
  * 
  * @return
  *   A pointer to the property created. It could return nullptr if there 
  *   is an error while trying to allocate memory.
  */
  Vec2Property*
  addProperty(const TString& name, 
              const Vector2D& value = {0.0f, 0.0f}, 
              const std::array<CHANNEL, 2>& readChannels = {kR, kG});

  /**
  * Adds a new property of Vector3D type to the material properperties.
  *
  * @param name
  *  The name of the property. If there was another property with that name
  *  in the material properties, it will be overwritten.
  *  
  * @param value
  *  The default value of the property, it will be mixed with the read value 
  *  of the texture if it exists.
  * 
  * @param readChannel
  *  The channel from which the information for the value of the property 
  *  will be taken.
  * 
  * @return
  *   A pointer to the property created. It could return nullptr if there 
  *   is an error while trying to allocate memory.
  */
  Vec3Property*
  addProperty(const TString& name, 
              const Vector3D& value = {0.0f, 0.0f, 0.0f}, 
              const std::array<CHANNEL, 3>& readChannels = {kR, kG, kB});

  /**
  * Adds a new property of Vector4D type to the material properperties.
  *
  * @param name
  *  The name of the property. If there was another property with that name
  *  in the material properties, it will be overwritten.
  *  
  * @param value
  *  The default value of the property, it will be mixed with the read value 
  *  of the texture if it exists.
  * 
  * @param readChannel
  *  The channel from which the information for the value of the property 
  *  will be taken.
  * 
  * @return
  *   A pointer to the property created. It could return nullptr if there 
  *   is an error while trying to allocate memory.
  */
  Vec4Property*
  addProperty(const TString& name, 
              const Vector4D& value = {0.0f, 0.0f, 0.0f, 0.0f}, 
              const std::array<CHANNEL, 4>& readChannels = {kR, kG, kB, kA});

  /**
  * Adds a new property to the material properperties.
  *
  * @param name
  *  The name of the property. If there was another property with that name
  *  in the material properties, it will be overwritten.
  *
  * @param property
  *  A pointer to the property to add.
  */
  //void
  //addProperty(const TString& name,
  //            PropertyPtr property);

  /**
  * Gets a property of type float.
  * 
  * @param name
  *  The name of the property sought.
  * 
  * @return
  *   The pointer to the specified property. If the property is found but 
  *   it is not of float type or if it's not found nullptr.
  */
  FloatProperty* 
  getFloatProperty(const TString& name);

  /**
  * Gets a property of type float.
  * 
  * @param name
  *  The name of the property sought.
  * 
  * @return
  *   The pointer to the specified property. If the property is found but 
  *   it is not of Vec2 type or if it's not found nullptr.
  */
  Vec2Property* 
  getVec2Property(const TString& name);

  /**
  * Gets a property of type float.
  * 
  * @param name
  *  The name of the property sought.
  * 
  * @return
  *   The pointer to the specified property. If the property is found but 
  *   it is not of Vec3 type or if it's not found nullptr.
  */
  Vec3Property* 
  getVec3Property(const TString& name);

  /**
  * Gets a property of type float.
  * 
  * @param name
  *  The name of the property sought.
  * 
  * @return
  *   The pointer to the specified property. If the property is found but 
  *   it is not of Vec4 type or if it's not found nullptr.
  */
  Vec4Property* 
  getVec4Property(const TString& name);

  /**
  * Removes a property from the materials properties.
  * 
  * @param name
  *  The name of the property to remove. If there isn't any property with
  *  this name nothing happens.
  */
  void
  removeProperty(const TString& name);

  /**
  * Gets the count of the materials properties.
  * 
  * @return
  *   The number of properties.
  */
  SizeT
  getPropertiesCount();

  /**
  * Gets a property in the materials properties.
  *
  * @param index
  *  The index of the material in the properties list.
  * 
  * @return
  *   The property at the specified index. If the index is greater
  *   or equal to the count of properties it will return nullptr.
  */
  Property*
  getProperty(SizeT index);

  /**
  * Gets a property in the materials properties.
  *
  * @param name
  *  The name of the material in the properties list.
  * 
  * @return
  *   The property at the specified index. If the index is greater
  *   or equal to the count of properties it will return nullptr.
  */
  Property*
  getProperty(const TString& name);

  Property*
  transformProperty(const TString& name, PROPERTY_TYPE newType);
 private:
  TString m_name;
  PropertyList m_properties;
};

}
