
#include <array>
#include <vector>
#include <dr_vector2d.h>
#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include <dr_memory.h>
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
    Property(const TString& name_, PROPERTY_TYPE type_) 
    : name(name_), 
      type(type_)
    {}

    virtual ~Property(){}
    
    TString name;
    const PROPERTY_TYPE type;
    //Texture* texture;
  };

  struct FloatProperty : Property
  {
    FloatProperty(const TString& name_, float value_, CHANNEL readChannel_)
    : Property(name_, kFloat),
      value(value_),
      readChannel(readChannel_)
    {}

    using ValueType = float;
    using ChannelType = CHANNEL;

    ValueType value;
    ChannelType readChannel;
  };

  struct Vec2Property : Property
  {
    Vec2Property(const TString& name_, 
                 const Vector2D& value_, 
                 const std::array<CHANNEL, 2>& readChannels_) 
    : Property(name_, kVec2),
      value(value_),
      readChannels(readChannels_)
    {
    }

    using ValueType = Vector2D;
    using ChannelType = std::array<CHANNEL, 2>;

    ValueType value;
    ChannelType readChannels;
  };

  struct Vec3Property : Property
  {
    Vec3Property(const TString& name_,
                 const Vector3D& value_, 
                 const std::array<CHANNEL, 3>& readChannels_) 
    : Property(name_, kVec3),
      value(value_),
      readChannels(readChannels_)
    {
    }

    using ValueType = Vector3D;
    using ChannelType = std::array<CHANNEL, 3>;

    ValueType value;
    ChannelType readChannels;  
  };

  struct Vec4Property : Property
  {
    Vec4Property(const TString& name_, 
                 const Vector4D& value_, 
                 const std::array<CHANNEL, 4>& readChannels_) 
    : Property(name_, kVec3),
      value(value_),
      readChannels(readChannels_)
    {
    }
 
    using ValueType = Vector4D;
    using ChannelType = std::array<CHANNEL, 4>;

    ValueType value;
    ChannelType readChannels;
  };

  using PropertyPtr = std::unique_ptr<Property>;
  using PropertyList = std::vector<PropertyPtr>;

  Material(const Material&) = delete;

  virtual ~Material(){}

  Material& operator=(const Material&) = delete;

  /**
  * Adds a new property of PropertyType type to the material properperties.
  * 
  * @param name
  *  The name of the property. 
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
  *   A pointer to the property created.
  */
  template<class PropertyType>
  PropertyType*
  addProperty(const TString& name, 
              typename const PropertyType::ValueType& value,
              typename const PropertyType::ChannelType& channel)
  {
    auto property = dr_make_unique<PropertyType>(name, value, channel);

    auto rawPtr = prop.get();
    
    m_properties.push_back(std::move(property));
    
    return rawPtr;
  }

  Property*
  addProperty(const TString& name, PROPERTY_TYPE type);
  
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
  * Gets a property of type PropertyType.
  * 
  * @param name
  *  The name of the property sought.
  * 
  * @return
  *   The pointer to the specified property. If the property is found but 
  *   it is not of PropertyType type or if it's not found nullptr.
  */
  template<class PropertyType>
  PropertyType* 
  getProperty(const TString& name)
  {
    PropertyType* rawPtr = nullptr;

    for (auto& property : m_properties) {
      if (property->name == name) {
        if (rawPtr = dynamic_cast<PropertyType*>(property.get())) {
          break;
        }
      }
    }

    return rawPtr;
  }

  ///**
  //* Gets a property of type PropertyType.
  //* 
  //* @param index
  //*  The index of the property sought.
  //* 
  //* @return
  //*   The pointer to the specified property. If the property is found but 
  //*   it is not of PropertyType type or if it's not found nullptr.
  //*/
  //template<class PropertyType>
  //PropertyType* 
  //getProperty(SizeT index)
  //{
  //  PropertyType rawPtr = nullptr;

  //  if (index < m_properties.size()) {
  //    rawPtr = dynamic_cast<PropertyType*>(m_properties[index].get());
  //  }

  //  return rawPtr;
  //}

  ///**
  //* Removes a property from the materials properties.
  //* 
  //* @param property
  //*  The property to remove.
  //*/
  //void
  //removeProperty(Property* property);

  /**
  * Removes a property from the materials properties.
  * 
  * @param name
  *  Name of the property to remove. If there are more then one properties
  *  with this name, the first whose matches it will be removed.
  */
  void
  removeProperty(const TString& name);

  /**
  * Removes a property from the materials properties.
  * 
  * @param index
  *  Index of the property to remove.
  */
  void
  removeProperty(SizeT index);

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
  *   The property with the specified name, if there are many properties
  *   with that name, the first found. If no property has the specified 
  *   name it'll retunr nullptr.
  */
  Property*
  getProperty(const TString& name);

  /**
  * Transforms the type of a property.
  *
  * @param name
  *  Name of the property to transform. If there are more than one properties
  *  with this name, the first whose matches the name will be transformed.
  * 
  * @param newType
  *  The type to transform the property. If the property found with the 
  *  specifed name has the same type, the function has no effect.
  *
  * @return 
  *   The new property transformed or the old property if no transformation
  *   takes place. If the property with the name specifed doesn't exist it 
  *   will return nullptr.
  */
  Property*
  transformProperty(const TString& name, PROPERTY_TYPE newType);

  /**
  * Transforms the type of a property.
  *
  * @param index
  *  Index of the property to transform.
  * 
  * @param newType
  *  The type to transform the property. If the property found with the 
  *  specifed index has the same type, the function has no effect.
  *
  * @return 
  *   The new property transformed or the old property if no transformation
  *   takes place. If the specified index is greater or equal to the propeties 
  *   count, nullptr.
  */
  Property*
  transformProperty(SizeT index, PROPERTY_TYPE newType);

  ///**
  //* Transforms the type of a property.
  //*
  //* @param property
  //*  The property to transform.
  //* 
  //* @param newType
  //*  The type to transform the property.
  //*
  //* @return 
  //*   The new property transformed or the old property if no transformation
  //*   takes place.
  //*/
  //Property*
  //transformProperty(Property* property, PROPERTY_TYPE newType);
 private:
   PropertyPtr
   createProperty(const TString& name, PROPERTY_TYPE type);
 private:
  TString m_name;
  PropertyList m_properties;
};

}