#pragma once
#include "dr_core_prerequisites.h"
#include <dr_resource.h>

#include <memory>

/**
* Structure with Decode and Encode functions.
*
* Sample usage:
*	Codec::decode("file_path/name_resource.png");
*/
namespace driderSDK {

namespace ResourceType {
 enum E
 {
  DR_TEXTURE,
  DR_MODEL,
  DR_SOUND,
  DR_SHADER,
  DR_MATERIAL 
 };
}

struct sImage
{
  Int32 w;
  Int32 h;
  Int32 channels;
  unsigned char* data;
};

class DR_CORE_EXPORT Codec
{
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
  virtual void*
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
  virtual void
  encode(TString pathName,
         ResourceType::E resourceType) = 0;

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
};
 

}