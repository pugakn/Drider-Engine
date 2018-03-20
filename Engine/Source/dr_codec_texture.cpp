#include "dr_codec_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb_image.h>
#include <dr_image_info.h>
#include <dr_memory.h>
#include <dr_string_utils.h>


namespace driderSDK {

CodecTexture::UniqueVoidPtr
CodecTexture::decode(TString pathName) {
  
  ImageInfo* image = nullptr;  
      
  Int32 width = 0;
  Int32 height = 0;
  Int32 channels = 0;

  auto data = stbi_load(StringUtils::toString(pathName).c_str(), 
                        &width, 
                        &height, 
                        &channels,
                        0);

  if (data) {
 
    image = new ImageInfo();

    image->width = width;
    image->height = height;
    image->channels = channels;
    
    SizeT imageSize = width * height * channels;

    image->data.resize(imageSize);

    std::memcpy(image->data.data(), data, imageSize);    
  }

  stbi_image_free(data);

  return UniqueVoidPtr(image, &dr_void_deleter<ImageInfo>);
}

bool
CodecTexture::encode(TString pathName) {
  
  return false;
}

bool
CodecTexture::isCompatible(TString extension) {
  //TString extension = FileSystem::getFileExtension(resourceName);
  if (extension == _T("png") || 
      extension == _T("jpg") || 
      extension == _T("jpeg") || 
      extension == _T("tga") ||
      extension == _T("hdr")) {
    return true;
  }
  return false;
}

CompatibleType::E
CodecTexture::getType() {
  return CompatibleType::TEXTURE;
}


}