#include <dr_engine_prerequisites.h>
#include <dr_codec_texture.h>
#include <dr_image_info.h>

#include <gtest\gtest.h>

TEST(Codec, decode) {
  EXPECT_TRUE(true);
}

TEST(Codec, encode) {
  EXPECT_TRUE(false);
}

TEST(Codec, isC) {
  driderSDK::CodecTexture odec;
  EXPECT_TRUE(odec.isCompatible(_T("testImage.jpg")));
  EXPECT_TRUE(odec.isCompatible(_T("testImage.png")));
  EXPECT_FALSE(odec.isCompatible(_T("testImage.mov")));
}