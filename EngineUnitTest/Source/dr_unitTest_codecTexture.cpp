#include <dr_engine_prerequisites.h>
#include <dr_codec_texture.h>
#include <dr_image_info.h>

#include <gtest\gtest.h>

TEST(Codec, decode) {
  driderSDK::CodecTexture odec;
  driderSDK::sImage *i = static_cast<driderSDK::sImage*>(odec.decode(_T("testImage.jpg")).get());
  EXPECT_TRUE(i->data.size() != 0);
  EXPECT_TRUE(i->channels != 0);
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