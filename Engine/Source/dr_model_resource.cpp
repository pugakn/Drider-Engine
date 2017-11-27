#include "dr_model_resource.h"

namespace driderSDK {

std::shared_ptr<Resource> __stdcall
ModelResource::Create() {
  auto resource = std::make_shared<ModelResource>();
  return resource;
}

}