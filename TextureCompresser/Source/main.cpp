#include "dr_TGA.h"

int
main(int argc, char* argv[]) {
  //1: Albedo
  //2: Normals
  //3: Metalic
  //4: Roughness;
  //5: Emissive
  //6: Transparency mask?
  if (argc < 2) {
    printf("No textures loaded.");
    return -1;
  }
  //driderSDK::Tga RedPixel("C:\\Users\\Francisco\\Desktop\\PixelRed.tga");
  //driderSDK::Tga BluePixel("C:\\Users\\Francisco\\Desktop\\PixelTest.tga");
  //driderSDK::Tga BluePixel("C:\\Users\\Francisco\\Desktop\\PixelTest.tga");
  //driderSDK::Tga BluePixel("C:\\Users\\Francisco\\Desktop\\CompressedPixelTest.tga");
  driderSDK::Tga BluePixel("C:\\Users\\Francisco\\Desktop\\Red10.tga");

  return 0;
}