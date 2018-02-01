#include <dr_resource_manager.h>
#include <gtest\gtest.h>

driderSDK::ResourceManager* resourceManager = new driderSDK::ResourceManager;
TEST(ResourceManager, resourceMConstructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, resourceMDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, init) {
  resourceManager->init();
  //EXPECT_TRUE(resourceManager->codecs.size() != 0);
}

TEST(ResourceManager, loadResource) {

  auto ref = resourceManager->loadResource(_T("testImage.png"));
  EXPECT_TRUE(ref != nullptr);
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