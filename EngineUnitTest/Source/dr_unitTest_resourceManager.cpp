#include <dr_resourceManager.h>
#include <gtest\gtest.h>

driderSDK::ResourceManager rm;

TEST(ResourceManager, resourceMConstructor) {
  driderSDK::ResourceManager r;
  EXPECT_TRUE(r.resourceContent.size() == 0);
}

TEST(ResourceManager, resourceMDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, loadResource) {
  rm.resourceContent.clear();
  rm.loadResource("test.png", "resources/images");
  EXPECT_TRUE(rm.resourceContent.size() > 0);
}

TEST(ResourceManager, createResource) { 
  rm.resourceContent.clear();
  rm.createResource(driderSDK::ResourceType::PNG, "test");
  EXPECT_TRUE(rm.resourceContent.size() > 0);
}

TEST(ResourceManager, isCompatible) {
  driderSDK::ResourceType::E type;
  EXPECT_TRUE(rm.isCompatible("resources/test.png", type));
  EXPECT_TRUE(type == driderSDK::ResourceType::PNG);
}

TEST(ResourceManager, existInResourceContent) {
  EXPECT_TRUE(rm.existInResourceContent("test"));
}