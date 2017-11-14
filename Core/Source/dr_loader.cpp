#include "dr_loader.h"
#include <comdef.h>

namespace driderSDK {

unsigned char*
Loader::texture(TString name,
                Int32 *width,
                Int32 *height,
                Int32 *nrChannels) {
  _bstr_t c(name.c_str());
  const char* a = c;
  return stbi_load(a, width, height, nrChannels, 0);
}

void
Loader::freeTexture(unsigned char* data) {
  stbi_image_free(data);
}

}