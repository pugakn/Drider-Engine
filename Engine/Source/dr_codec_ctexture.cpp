#include "dr_codec_ctexture.h"
#include <dr_image_info.h>
#include <dr_memory.h>
#include <dr_string_utils.h>
#include <fstream>
#include <string>
#include <stdlib.h>
namespace driderSDK {

  CodecCompressedTexture::UniqueVoidPtr
    CodecCompressedTexture::decode(TString pathName) {

    ImageInfo* image = nullptr;

    Int32 width = 0;
    Int32 height = 0;
    Int32 channels = 0;
    ///////
    char header[124];


    std::ifstream file;
    file.open(pathName, std::ios::binary);

    char filecode[4];
    file.read(filecode,4);
    std::string codestr(filecode,3);


    if (codestr != "DDS") {
      file.close();
      return false;
    }

    file.read(header,124);
    width = *(UInt32*)&(header[8]);
    height = *(UInt32*)&(header[12]);

    UInt32 linearSize = *(UInt32*)&(header[16]);
    UInt32 mipMapCount = *(UInt32*)&(header[24]);
    UInt32 fourCC = *(UInt32*)&(header[80]);
    channels = (fourCC == TextureCompressionType::FOURCC_DXT1) ? 3 : 4;

    UInt32 bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    byte* data = (byte*)malloc(bufsize * sizeof(byte));

    file.read((char*)data, bufsize);
    file.close();



    ///////

    if (data) {
      image = new ImageInfo();

      image->width = width;
      image->height = height;
      image->channels = channels;
      image->compressionType = fourCC;
      image->mipMapCount = mipMapCount;

      SizeT imageSize = ((width + 3) / 4)*((height + 3) / 4)*((fourCC == TextureCompressionType::FOURCC_DXT1) ? 8 : 16);

      image->data.resize(imageSize);

      std::memcpy(image->data.data(), data, imageSize);
      free(data);
    }

    return UniqueVoidPtr(image, &dr_void_deleter<ImageInfo>);
  }

  bool
    CodecCompressedTexture::encode(TString pathName) {

    return false;
  }

  bool
    CodecCompressedTexture::isCompatible(TString extension) {
    //TString extension = FileSystem::getFileExtension(resourceName);
    if (extension == _T("dds")) {
      return true;
    }
    return false;
  }

  CompatibleType::E
    CodecCompressedTexture::getType() {
    return CompatibleType::COMPRESSED_TEXTURE;
  }


}