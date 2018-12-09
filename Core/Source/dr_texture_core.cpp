#include "dr_texture_core.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_texture.h>
#include "dr_image_info.h"
namespace driderSDK {

TextureCore::TextureCore() {}

TextureCore::~TextureCore() {}

void
TextureCore::init(void* data) {
  ImageInfo* image = static_cast<ImageInfo*>(data);

  DrTextureDesc desc;

  desc.width = image->width;
  desc.height = image->height;
  desc.pitch = image->width * image->channels;
  desc.dimension = DR_DIMENSION::k2D; 
  //desc.dimension = image->textureDimension;
  desc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE;
  desc.genMipMaps = true;
  desc.mipLevels = 1;
  if (image->compressionType == 0) {
    if (1 == image->channels) {
      desc.Format = DR_FORMAT::kR8_UNORM;
    }
    else if (2 == image->channels) {
      desc.Format = DR_FORMAT::kR8G8_UNORM;
    }
    else {
      desc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
    }
  }
  else {
    if (image->compressionType == TextureCompressionType::FOURCC_DXT1) {  //BTC1
      desc.Format = DR_FORMAT::kBC1_UNORM;
      desc.pitch = ((image->width + 3) / 4) * 8;
    }
    else if (image->compressionType == TextureCompressionType::FOURCC_DXT3) {  //BTC2
      desc.Format = DR_FORMAT::kBC2_UNORM;
      desc.pitch = ((image->width + 3) / 4) * 16;
    }
    else if (image->compressionType == TextureCompressionType::FOURCC_DXT5) {  //BTC3
      desc.Format = DR_FORMAT::kBC3_UNORM;
      desc.pitch = ((image->width + 3) / 4) * 16;
    }
    desc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drWrite;
    desc.mipLevels = image->mipMapCount;
  }


 /* switch (image->channels) {
  case 1:
    desc.Format = DR_FORMAT::kDrFormat_R8_UNORM;
  break;
  case 2:
    desc.Format = DR_FORMAT::kDrFormat_R8G8_UNORM;
  break;
  case 3:
    desc.Format = DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
  break;
  case 4:
    desc.Format = DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
  break;
  default:
  DR_ASSERT(false);
  break;
  }*/

  auto cdata = reinterpret_cast<char*>(image->data.data());

  auto t = GraphicsAPI::getDevice().createEmptyTexture(desc);

  t->udpateFromMemory(GraphicsAPI::getDeviceContext(), 
                      cdata, 
                      image->data.size()//,
                      );//image->mipMapCount

  textureGFX = dr_unique_custom<>(t, dr_gfx_deleter<Texture>);
}

void
TextureCore::init(void* data, void* extraData) {
  ImageInfo* image = static_cast<ImageInfo*>(data);
  ImageInfo* imageExtraData = static_cast<ImageInfo*>(extraData);

  DrTextureDesc desc;

  desc.width = image->width;
  desc.height = image->width;
  desc.pitch = image->width * image->channels;
  desc.dimension = imageExtraData->textureDimension;
  desc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE;
  desc.genMipMaps = true;
  desc.mipLevels = 1;
  desc.Format = DR_FORMAT::kR8G8B8A8_UNORM;

  auto cdata = reinterpret_cast<char*>(image->data.data());

  auto t = GraphicsAPI::getDevice().createEmptyTexture(desc);

  t->udpateFromMemory(GraphicsAPI::getDeviceContext(),
    cdata,
    image->data.size());

  textureGFX = dr_unique_custom<>(t, dr_gfx_deleter<Texture>);
}

}