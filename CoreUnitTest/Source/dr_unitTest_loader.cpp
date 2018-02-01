#include <dr_loader.h>

#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>


TEST(Loader, texture) {
  driderSDK::Int32 w;
  driderSDK::Int32 h;
  driderSDK::Int32 channels;
  unsigned char* data = driderSDK::Loader::texture(_T("testImage.png"),
                                                   &w,
                                                   &h,
                                                   &channels);

  EXPECT_TRUE(data != nullptr);

  driderSDK::Loader::freeTexture(data);

  EXPECT_TRUE(data != nullptr);
}