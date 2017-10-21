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
  static ResourceType::E
  decode(std::string pathName);

  static std::shared_ptr<Resource>
  encode(std::string pathName);
};
 

}