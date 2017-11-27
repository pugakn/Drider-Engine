#include "dr_texture_resource.h"

namespace driderSDK {

std::shared_ptr<Resource> __stdcall
TextureResource::Create() {
  auto resource = std::make_shared<TextureResource>();
  return resource;
}

}