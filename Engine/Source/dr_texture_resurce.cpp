#include "dr_texture_resource.h"
#include "dr_codec_texture.h"
#include "dr_image_info.h"
#include <dr_memory.h>

using driderSDK::Codec;
using driderSDK::CodecTexture;
using CodecPtr = std::unique_ptr<Codec>;

namespace driderSDK {


void
TextureResource::init(void* pData) {
  sImage* pInfo = static_cast<sImage*>(pData);
  channels = pInfo->channels;
  data = pInfo->data;
  width = pInfo->w;
  height = pInfo->h;
}

}