#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include "dr_engine_prerequisites.h"
#include "dr_compatible_types.h"

namespace driderSDK {

class Resource;

class DR_ENGINE_EXPORT ResourceFactory {
 public:

  using ResourceCreator = std::function<std::shared_ptr<Resource>()>;

  /**
  * TEST::resourceFacturyConstructor
  * Default constructor
  */
  ResourceFactory();

  /**
  * TEST::resourceFactoryDestructor
  * Default constructor
  */
  ~ResourceFactory() { m_FactoryMap.clear(); }
  
  /**
  * TEST::resourceFactoryConstructor
  * Record the functions of the resources
  * 
  * @param type
  *   The type of the resource
  *
  * @param pfnCreate
  *   Create resource function
  *
  */
  void 
  Register(CompatibleType::E type, 
           ResourceCreator pfnCreate);

  /*
  * TEST::createResource
  * Creates the specific Resource
  *
  * @param type
  *  The type of the resource to create
  * 
  * @return
  *   shared_ptr to the Resource creates
  */
  std::shared_ptr<Resource>
  CreateResource(CompatibleType::E type);

 public:
   typedef std::unordered_map<CompatibleType::E, ResourceCreator> FactoryMap;
   FactoryMap m_FactoryMap;
};
}