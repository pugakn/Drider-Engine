#include <dr_resource_manager.h>
#include <gtest\gtest.h>

TEST(ResourceManager, resourceMConstructor) {
  driderSDK::ResourceManager* resourceManager = new driderSDK::ResourceManager;
  resourceManager->loadResource(L"imageTest.png");
  EXPECT_TRUE(false);
}

TEST(ResourceManager, resourceMDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, loadResource) {
  EXPECT_TRUE(false);
}

TEST(ResourceManager, createResource) { 
  
  EXPECT_TRUE(false);
}

TEST(ResourceManager, existInResourceContent) {
  EXPECT_TRUE(false);
}

TEST(ResourceManager, getReference) {
  EXPECT_TRUE(false);
}