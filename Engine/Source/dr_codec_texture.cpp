#include "dr_codec_texture.h"
#include <comdef.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb_image.h>
#include <dr_image_info.h>
#include <dr_memory.h>
#include <dr_parser.h>

namespace driderSDK {

CodecTexture::UniqueVoidPtr
CodecTexture::decode(TString pathName) {
  sImage *image = new sImage();  

  _bstr_t c(pathName.c_str());
  const char* a = c;
  unsigned char* data = stbi_load(a, &image->w, &image->h, &image->channels, 0);
  stbi_image_free(data);
  return UniqueVoidPtr(image, &void_deleter<sImage>);
}

bool
CodecTexture::encode(TString pathName) {
  
  return false;
}

bool
CodecTexture::isCompatible(TString resourceName) {
 /* TString extension = Parser::(resourceName, _T("."), _T(" "), extension);
  if(extension == _T("png") || extension == _T("jpg")) {
    return true;
  }*/
  return false;
}


}