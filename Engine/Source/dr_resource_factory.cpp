#include "dr_resource_factory.h"
#include "dr_texture_resource.h"
#include "dr_model_resource.h"

namespace driderSDK {

ResourceFactory::ResourceFactory() {
  Register(CompatibleType::TEXTURE, TextureResource::Create);
  Register(CompatibleType::MODEL, ModelResource::Create);
}

void
ResourceFactory::Register(CompatibleType::E type,
                          CreateResourceFn pfnCreate) {
  m_FactoryMap[type] = pfnCreate;
}


std::shared_ptr<Resource>
ResourceFactory::CreateResource(CompatibleType::E type) {
  FactoryMap::iterator it = m_FactoryMap.find(type);
  if (it != m_FactoryMap.end())
    return it->second();
  return NULL;
}


}