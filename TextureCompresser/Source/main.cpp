#include "dr_TGA.h"

int
main(int argc, char* argv[]) {
  //1: Albedo
  //2: Normals
  //3: Metalic
  //4: Roughness;
  //5: Emissive
  //6: Transparency mask
  //if (argc < 2) {
  //  printf("No textures loaded.");
  //  return -1;
  //}

  driderSDK::TGA RedTesto;
  RedTesto.create(8, 8);
  driderSDK::Pixel RedGradient = driderSDK::Pixel::Black;

  driderSDK::UInt8 jump = 256 / (8 * 8);

  for (driderSDK::SizeT index = 0; index < 64; ++index) {
    RedTesto.GetPixel(index) = RedGradient;
    RedGradient.r += jump;
  }

  RedTesto.save(_T("C:\\Users\\Francisco\\Desktop\\RedGradient.tga"));

  return 0;

  driderSDK::TGA TestAlbedo;
  driderSDK::TGA TestEmissive;
  driderSDK::TGA TestNormals;
  driderSDK::TGA TestMetallic;
  driderSDK::TGA TestRoughness;
  driderSDK::TGA TestAlphaMask;

  bool hasAlbedoTexture     = TestAlbedo.load(_T("C:\\Users\\Francisco\\Desktop\\TestAlbedo.tga"));
  bool hasEmissiveTexture   = TestEmissive.load(_T("C:\\Users\\Francisco\\Desktop\\TestEmissive.tga"));
  bool hasNormalsTexture    = TestNormals.load(_T("C:\\Users\\Francisco\\Desktop\\TestNormals.tga"));
  bool hasMetallicTexture   = TestMetallic.load(_T("C:\\Users\\Francisco\\Desktop\\TestMetallic.tga"));
  bool hasRoughnessTexture  = TestRoughness.load(_T("C:\\Users\\Francisco\\Desktop\\TestRoughness.tga"));
  bool hasAlphaMaskTexture  = TestAlphaMask.load(_T("C:\\Users\\Francisco\\Desktop\\TestAlphaMask.tga"));

  
  TestAlbedo.save(_T("C:\\Users\\Francisco\\Desktop\\TestAlbedoCompressed.tga"));
  TestEmissive.save(_T("C:\\Users\\Francisco\\Desktop\\TestEmissiveCompressed.tga"));
  TestNormals.save(_T("C:\\Users\\Francisco\\Desktop\\TestNormalsCompressed.tga"));
  TestMetallic.save(_T("C:\\Users\\Francisco\\Desktop\\TestMetallicCompressed.tga"));
  TestRoughness.save(_T("C:\\Users\\Francisco\\Desktop\\TestRoughnessCompressed.tga"));
  TestAlphaMask.save(_T("C:\\Users\\Francisco\\Desktop\\TestAlphaMaskCompressed.tga"));

  return 0;
}