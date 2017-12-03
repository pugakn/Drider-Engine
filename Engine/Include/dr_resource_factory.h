#pragma once
#include <dr_engine_prerequisites.h>
#include <memory>
#include <unordered_map>
#include <dr_resource.h>
#include <dr_compatible_types.h>

namespace driderSDK {
class Codec;
class DR_ENGINE_EXPORT ResourceFactory {
 public:

  typedef std::shared_ptr<Resource>(__stdcall *CreateResourceFn)(TString);

  /**
  * TEST::resourceFacturyConstructor
  * Default constructor
  */
  ResourceFactory();

  /**
  * TEST::resourceFacturyDestructor
  * Default constructor
  */
  ~ResourceFactory() { m_FactoryMap.clear(); }
  
  /**
  * TEST::resourceFacturyConstructor
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
           CreateResourceFn pfnCreate);

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
  CreateResource(Codec *codec,
                 TString resourceName);

 public:
   typedef std::unordered_map<CompatibleType::E, CreateResourceFn> FactoryMap;
   FactoryMap m_FactoryMap;
};
}