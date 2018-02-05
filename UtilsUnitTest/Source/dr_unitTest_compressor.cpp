
#include <gtest\gtest.h>
#include <dr_compressor.h>
TEST (Compressor, compress) {
  char xd[10];
  char dest[1000];
  for (int i = 0; i < 10; i++) {
    xd[i] = i;
  }
  int ret = driderSDK::Compressor::compress((driderSDK::byte*)xd,(driderSDK::byte*)dest,10, 1000);
  EXPECT_TRUE(ret > 0);
}

TEST(Compressor, decompress) {
  char xd[10];
  char dest[1000];
  for (int i = 0; i < 10; i++) {
    xd[i] = i;
  }
  int ret = driderSDK::Compressor::compress((driderSDK::byte*)xd, (driderSDK::byte*)dest, 10, 1000);
  int ret2 = driderSDK::Compressor::decompress((driderSDK::byte*)dest, (driderSDK::byte*)xd, ret, 10);
  EXPECT_TRUE(ret2 == 10);
}