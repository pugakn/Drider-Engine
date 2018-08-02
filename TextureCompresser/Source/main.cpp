#include "dr_TextureCompressor.h"
#include "dr_TGA.h"

int
main(int argc, char* argv[]) {
  //1: Albedo
  //2: Normals
  //3: Metalic
  //4: Roughness;
  //5: Emissive
  //6: Transparency mask
  if (argc < 2) {
  //  printf("No textures loaded.");
  //  return -1;
  }

  driderSDK::TGA TestAlbedo;    //1
  driderSDK::TGA TestNormals;   //2
  driderSDK::TGA TestMetallic;  //3
  driderSDK::TGA TestRoughness; //4
  driderSDK::TGA TestEmissive;  //5
  driderSDK::TGA TestAlphaMask; //6

  driderSDK::TGA compressedNormals;
  driderSDK::TGA unCompressedNormals;



  //bool hasAlbedoTexture     = TestAlbedo.load(_T("C:\\Users\\Francisco\\Desktop\\TestAlbedo.tga"));
  //bool hasEmissiveTexture   = TestEmissive.load(_T("C:\\Users\\Francisco\\Desktop\\TestEmissive.tga"));
  bool hasNormalsTexture    = TestNormals.load(_T("C:\\Users\\Francisco\\Desktop\\TestNormals.tga"));
  if (!hasNormalsTexture) {
    return -1;
  }
  //bool hasMetallicTexture   = TestMetallic.load(_T("C:\\Users\\Francisco\\Desktop\\TestMetallic.tga"));
  //bool hasRoughnessTexture  = TestRoughness.load(_T("C:\\Users\\Francisco\\Desktop\\TestRoughness.tga"));
  //bool hasAlphaMaskTexture  = TestAlphaMask.load(_T("C:\\Users\\Francisco\\Desktop\\TestAlphaMask.tga"));


  driderSDK::Int32 texWidth = static_cast<driderSDK::Int32>(TestNormals.GetWidth());
  driderSDK::Int32 texHeight = static_cast<driderSDK::Int32>(TestNormals.GetHeight());
  driderSDK::Int32 texSize = texWidth * texHeight;

  driderSDK::Vector2D SphereCoords;
  driderSDK::Vector3D CartesianCoords;

  compressedNormals.create(texWidth, texHeight);
  unCompressedNormals.create(texWidth, texHeight);

  driderSDK::Pixel actualPixel;
  for (driderSDK::Int32 i = 0; i < texSize; ++i) {
    actualPixel = TestNormals.GetPixel(i);

    CartesianCoords = driderSDK::TextureCompressor::color2Vector3D(actualPixel);
    CartesianCoords /= 255.0f;
    CartesianCoords = (CartesianCoords * 2.0f) - driderSDK::Vector3D(1.0f, 1.0f, 1.0f);
    CartesianCoords.normalize();

    SphereCoords = driderSDK::TextureCompressor::Cartesian2Spherical(CartesianCoords);

    actualPixel.r = driderSDK::TextureCompressor::float2int(SphereCoords.x);
    actualPixel.g = driderSDK::TextureCompressor::float2int(SphereCoords.y);
    actualPixel.b = static_cast<driderSDK::UInt8>(0);

    CartesianCoords.x = driderSDK::TextureCompressor::int2float(actualPixel.r);
    CartesianCoords.y = driderSDK::TextureCompressor::int2float(actualPixel.g);
    CartesianCoords.z = 0.0f;

    actualPixel = driderSDK::TextureCompressor::Vector3D2Color(driderSDK::Vector3D(SphereCoords.x, SphereCoords.y, 0.0f));

    compressedNormals.GetPixel(i) = actualPixel;
  }

  //driderSDK::TextureCompressor::Cartesian2Spherical();

  
  compressedNormals.save(_T("C:\\Users\\Francisco\\Desktop\\CompressedNormals.tga"));
  unCompressedNormals.save(_T("C:\\Users\\Francisco\\Desktop\\UncompressedNormals.tga"));

  //TestAlbedo.save(_T("C:\\Users\\Francisco\\Desktop\\TestAlbedoCompressed.tga"));
  //TestEmissive.save(_T("C:\\Users\\Francisco\\Desktop\\TestEmissiveCompressed.tga"));
  //TestNormals.save(_T("C:\\Users\\Francisco\\Desktop\\TestNormalsCompressed.tga"));
  //TestMetallic.save(_T("C:\\Users\\Francisco\\Desktop\\TestMetallicCompressed.tga"));
  //TestRoughness.save(_T("C:\\Users\\Francisco\\Desktop\\TestRoughnessCompressed.tga"));
  //TestAlphaMask.save(_T("C:\\Users\\Francisco\\Desktop\\TestAlphaMaskCompressed.tga"));

  return 0;
}