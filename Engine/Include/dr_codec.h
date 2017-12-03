#pragma once
#include <memory>
#include <functional>
#include "dr_engine_prerequisites.h"
#include "dr_compatible_types.h"

/**
* Structure with Decode and Encode functions.
*
* Sample usage:
*	Codec::decode("file_path/name_resource.png");
*/

namespace driderSDK {

class DR_ENGINE_EXPORT Codec
{
 public:

  using VoidPtrDeleter = std::function<void(void*)>;
  using UniqueVoidPtr = std::unique_ptr<void, VoidPtrDeleter>;
  
  /**
  * 
  */ 
  Codec() {}

  
  /**
  *
  */
  virtual ~Codec() {}

  /**
  * TEST::codecDecode
  * Decodes and load a resource.
  *
  * @param pathName
  *   Path of the file (include extension).
  *
  * @return
  *   The type of the resource ResourceType::E.
  */
  virtual UniqueVoidPtr
  decode(TString pathName) = 0;

  /**
  * TEST::codecEncode
  * Encode and save a resource.
  *
  * @param pathName
  *   Path of the file (include extension).
  *
  * @return
  *   Shared_ptr to a Resource.
  */
  virtual bool
  encode(TString pathName) = 0;

  /**
  * TEST::isCompatible
  * Checks if a extension is compatible
  * 
  * @param resourceName
  * The name of the resource, includ extension
  *
  * @return
  * Returns true if the resource is compatible, otherwise return false
  */
  virtual bool
  isCompatible (TString resourceName) = 0;

  /**
  *
  */
  virtual CompatibleType::E
  getType () = 0;
};
 

}