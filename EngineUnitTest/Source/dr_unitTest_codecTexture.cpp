#include <dr_codecTexture.h>

#include <gtest\gtest.h>

TEST(Codec, decode) {
  driderSDK::CodecTexture odec;
  odec.decode(_T("test.png"));
  EXPECT_TRUE(false);
}

TEST(Codec, encode) {
  EXPECT_TRUE(false);
}

TEST(Codec, isC) {
  EXPECT_TRUE(false);
}