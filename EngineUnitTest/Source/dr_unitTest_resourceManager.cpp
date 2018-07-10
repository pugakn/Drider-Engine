#include <dr_resource_manager.h>
#include <gtest\gtest.h>

driderSDK::ResourceManager* resourceManager;
TEST(ResourceManager, resourceMConstructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, resourceMDestructor) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, init) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, loadResource) {

  if (!driderSDK::ResourceManager::isStarted()) {
    driderSDK::ResourceManager::startUp();
  }

  auto ref = driderSDK::ResourceManager::loadResource(_T("Sound1.mp3"));
  ref = driderSDK::ResourceManager::loadResource(_T("Sound2.mp3"));
  ref = driderSDK::ResourceManager::loadResource(_T("Script1.as"));
  ref = driderSDK::ResourceManager::loadResource(_T("Script2.as"));
  EXPECT_TRUE(ref != nullptr);
}

TEST(ResourceManager, createResource) { 
  
  EXPECT_TRUE(true);
}

TEST(ResourceManager, existInResourceContent) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, getReference) {
  EXPECT_TRUE(true);
}

TEST(ResourceManager, getAllResourcesOfType) {
  
  
  EXPECT_TRUE(true);
}