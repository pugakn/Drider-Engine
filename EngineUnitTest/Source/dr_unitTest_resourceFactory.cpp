#include <dr_resource_factory.h>
#include <dr_texture_resource.h>

#include <gtest\gtest.h>

auto factory = std::make_shared<driderSDK::ResourceFactory>();

TEST(ResourceFactory, resourceFactoryConstructor) {
  EXPECT_TRUE(factory != nullptr);
  EXPECT_TRUE(factory->m_FactoryMap.size() != 0);
}

TEST(ResourceFactory, resourceFactoryDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceFactory, CreateResource) {
  
  std::shared_ptr<driderSDK::Resource> r;
  r = factory->CreateResource(driderSDK::CompatibleType::TEXTURE);

  std::shared_ptr<driderSDK::TextureResource> Texture;
  Texture = std::static_pointer_cast<driderSDK::TextureResource>(r);
  Texture->width = 10;
  EXPECT_TRUE(Texture->width == 10);
}