#include "dr_resource_factory.h"
#include <dr_model.h>
#include "dr_texture_resource.h"

namespace driderSDK {

ResourceFactory::ResourceFactory() {
  Register(CompatibleType::TEXTURE, 
           std::make_shared<TextureResource>);
  Register(CompatibleType::MODEL, 
           std::make_shared<Model>);
}

void
ResourceFactory::Register(CompatibleType::E type,
                          ResourceCreator pfnCreate) {
  m_FactoryMap[type] = pfnCreate;
}


std::shared_ptr<Resource>
ResourceFactory::CreateResource(CompatibleType::E type) {
  FactoryMap::iterator it = m_FactoryMap.find(type);
  if (it != m_FactoryMap.end())
    return it->second();
  return nullptr;
}


}