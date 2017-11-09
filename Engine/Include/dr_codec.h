#pragma once
#include <dr_prerequisites.h>
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
  UNSUPPORTED,
  PNG,
  JPG,
  OBJ_MODEL,
  FBX_MODEL,
  DR_SHADER,
  DR_MAT  
 };
}

struct DR_API_EXPORT Codec
{
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
  static std::shared_ptr<Resource>
  decode(TString pathName);

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
  static void
  encode(TString pathName,
         ResourceType::E resourceType);

  /**
  * TEST::isCompatible
  */
  static ResourceType::E
  isCompatible (TString pathName);
};
 

}