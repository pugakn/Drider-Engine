#include <dr_texture_resource.h>

#include <gtest\gtest.h>

TEST(TextureResource, constructor) {
  driderSDK::TextureResource texture;
  texture.width = 10;
  EXPECT_TRUE(texture.width == 10);
}

TEST(TextureResource, destructor) {
  EXPECT_TRUE(true);
}

TEST(TextureResource, create) {
  driderSDK::TextureResource texture;
  std::shared_ptr<driderSDK::Resource> ptr;
  
  ptr = texture.Create();

  EXPECT_TRUE(ptr != nullptr);
}