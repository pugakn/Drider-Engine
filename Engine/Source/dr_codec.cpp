#include "dr_codec.h"

namespace driderSDK {
  
ResourceType::E
Codec::decode(std::string pathName) {
 return ResourceType::PNG;
}

std::shared_ptr<Resource>
Codec::encode(std::string pathName) {
 return std::shared_ptr<Resource> ();
}

}