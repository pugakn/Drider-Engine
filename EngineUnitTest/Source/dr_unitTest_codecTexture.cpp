#include <dr_codec_texture.h>
#include <gtest\gtest.h>

TEST(Codec, decode) {
  driderSDK::CodecTexture odec;
  odec.decode(_T("testImage.jpg"));
  EXPECT_TRUE(false);
}

TEST(Codec, encode) {
  EXPECT_TRUE(false);
}

TEST(Codec, isC) {
  EXPECT_TRUE(false);
}