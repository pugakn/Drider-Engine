#pragma once
#include <memory>
#include <dr_prerequisites.h>
#include <dr_resource.h>
#include "dr_codec.h"

namespace driderSDK {

class DR_API_EXPORT ResourceManager {
 public:
  /**
  *	Default constructor.
  */
  ResourceManager() {}

  /**
  *	Default destructor.
  */
  ~ResourceManager() {}
  
  // Carga un recurso, lo inserta en resourceContent y retorna la llave
  void
  loadResource (std::string nameResource,
                std::string pathFile);
  
  // Crea un recuso en el resourceContent y le asigna una llave
  void
  createResource(ResourceType::E resourceType, std::string resourceName);
  
  // Revisa si un recurso es compatible, de ser asi retorna true y regresa el tipo de
  // recurso en resourceType 
  bool
  isCompatible(std::string resource,
               ResourceType::E &resourceTypeOut);

  bool
  existInResourceContent (std::string key);

  std::shared_ptr<Resource>
  getReference(std::string key);
  
 std::map<std::string, std::shared_ptr<Resource>> resourceContent;


};

}