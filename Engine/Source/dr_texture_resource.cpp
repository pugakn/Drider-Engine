#include "dr_texture_resource.h"
#include "dr_codec_texture.h"
#include "dr_image_info.h"
#include <dr_memory.h>

using driderSDK::Codec;
using driderSDK::CodecTexture;
using CodecPtr = std::unique_ptr<Codec>;

namespace driderSDK {

std::shared_ptr<Resource> __stdcall
TextureResource::Create(TString resourceName) {

  CodecPtr textureCodec = dr_make_unique<CodecTexture>();
  Codec::UniqueVoidPtr pData = textureCodec->decode(resourceName);
  sImage* pInfo = static_cast<sImage*>(pData.get());
  auto resource = std::make_shared<TextureResource>();
  resource->channels = pInfo->channels;
  resource->data = pInfo->data;
  resource->width = pInfo->w;
  resource->height = pInfo->h;
  return resource;
}

}