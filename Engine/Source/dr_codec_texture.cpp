#include "dr_codec_texture.h"
#include <dr_image_info.h>
#include <dr_memory.h>
#include <dr_string_utils.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

namespace driderSDK {

CodecTexture::UniqueVoidPtr
CodecTexture::decode(TString pathName) {
  
  ImageInfo* image = nullptr;  
      
  Int32 width = 0;
  Int32 height = 0;
  Int32 channels = 0;

  TString realPath = _T("Resources\\Textures\\") + pathName;

  auto data = stbi_load(StringUtils::toString(realPath).c_str(),
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
CodecTexture::encodeImage(String filename,
                          Int32 width,
                          Int32 height,
                          DR_FILE_FORMAT::E fileFormat,
                          std::vector<byte>* data) {
  if (DR_FILE_FORMAT::TGA == fileFormat) {
    return stbi_write_tga(filename.append(".tga").c_str(),
                          width,
                          height,
                          STBI_rgb_alpha,
                          &(*data)[0]);
  }
  else if (DR_FILE_FORMAT::BMP == fileFormat) {
    return stbi_write_bmp(filename.append(".bmp").c_str(),
                          width,
                          height,
                          STBI_rgb_alpha,
                          &(*data)[0]);
  }
  else if (DR_FILE_FORMAT::DDS == fileFormat) {
    return false;
  }
  else if (DR_FILE_FORMAT::JPG == fileFormat) {
    return stbi_write_jpg(filename.append(".jpg").c_str(),
                          width,
                          height,
                          STBI_rgb_alpha,
                          &(*data)[0],
                          95);
  }
  else if (DR_FILE_FORMAT::PNG == fileFormat) {
    return stbi_write_png(filename.append(".png").c_str(),
                          width,
                          height,
                          STBI_rgb_alpha,
                          &(*data)[0],
                          data->size() / height);
  }

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