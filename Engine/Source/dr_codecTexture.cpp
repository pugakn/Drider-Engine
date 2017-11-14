#include "dr_codecTexture.h"
#include <dr_parser.h>

namespace driderSDK {

void*
CodecTexture::decode(TString pathName) {
  sImage *image = new sImage();  

  _bstr_t c(pathName.c_str());
  const char* a = c;
  unsigned char* data = stbi_load(a, &image->w, &image->h, &image->channels, 0);
  //std::strncpy((char*)&image.data, (char*)&data, strlen((const char*)&data));
  stbi_image_free(data);
  return (void*)(image);
}

bool
CodecTexture::encode(TString pathName) {
  int a = 0;
  return false;
}

bool
CodecTexture::isCompatible(TString resourceName) {
  TString extension = Parser::strCpyBetween(resourceName, _T("."), _T(" "), extension);
  if(extension == _T("png") || extension == _T("jpg")) {
    return true;
  }
  return false;
}


}