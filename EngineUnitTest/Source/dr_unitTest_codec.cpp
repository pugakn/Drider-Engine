#include <dr_codec.h>
#include <dr_resource.h>

#include <gtest\gtest.h>

TEST (Codec, codecDecode) {
  EXPECT_TRUE(driderSDK::Codec::decode("images.png") ==
              driderSDK::ResourceType::PNG);
}

TEST(Codec, codecEncode) {
  EXPECT_TRUE(driderSDK::Codec::encode("images.png") ==
              std::shared_ptr<driderSDK::Resource>());
}