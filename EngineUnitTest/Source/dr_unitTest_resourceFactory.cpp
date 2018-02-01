#include <dr_resource_factory.h>
#include <dr_texture_resource.h>

#include <gtest\gtest.h>

auto factory = new driderSDK::ResourceFactory;

TEST(ResourceFactory, resourceFactoryConstructor) {
  EXPECT_TRUE(factory->m_FactoryMap.size() != 0);
}

TEST(ResourceFactory, resourceFactoryDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceFactory, CreateResource) { 
  EXPECT_TRUE(false);
}