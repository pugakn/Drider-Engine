#pragma once
#include <dr_prerequisites.h>
#include <dr_resource.h>
#include <memory>

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
  static ResourceType::E
  decode(std::string pathName);

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
  static std::shared_ptr<Resource>
  encode(std::string pathName);
};
 

}