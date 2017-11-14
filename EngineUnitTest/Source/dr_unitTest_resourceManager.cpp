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
  rm.loadResource(_T("test.png"));
  EXPECT_TRUE(rm.resourceContent.size() > 0);
}

TEST(ResourceManager, createResource) { 
  rm.resourceContent.clear();
  rm.createResource(driderSDK::ResourceType::PNG, "test");
  EXPECT_TRUE(rm.resourceContent.size() > 0);
}

TEST(ResourceManager, existInResourceContent) {
  EXPECT_TRUE(rm.existInResourceContent("test"));
}

TEST(ResourceManager, getReference) {
  rm.loadResource(_T("test.png"));
  rm.createResource(driderSDK::ResourceType::PNG, "test");
  EXPECT_TRUE(rm.getReference("test") == nullptr);
  EXPECT_FALSE(true);
}