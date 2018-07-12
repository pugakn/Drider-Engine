#pragma once

#include <vector>
#include <dr_graphics_defines.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {
  namespace TextureCompressionType {
    enum E {
     FOURCC_DXT1 = 0x31545844, //(MAKEFOURCC('D','X','T','1'))
     FOURCC_DXT3 = 0x33545844, //(MAKEFOURCC('D','X','T','3'))
     FOURCC_DXT5 = 0x35545844 //(MAKEFOURCC('D','X','T','5'))
    };
  }
struct DR_CORE_EXPORT ImageInfo
{
  UInt32 width;
  UInt32 height;
  UInt32 channels;
  UInt32 mipMapCount = 0;
  UInt64 compressionType = 0;
  DR_DIMENSION::E textureDimension;
  std::vector<UInt8> data;
};

}
