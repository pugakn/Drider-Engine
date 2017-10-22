#pragma once
#include <dr_prerequisites.h>
namespace driderSDK {
namespace DR_BUFFER_USAGE {
  enum E
  {
    kDefault,
    kImmutable,
    kDynamic,
    kStaging
  };
}

namespace DR_FORMAT {
  enum E
  {
    kR8_float,
    kR16_float,
    kR32_float,
    kR4G4B4_float,
    kR4G4B4A4_float,
    kR8G8B8_float,
    kR8G8B8A8_float,
    kR16G16B16_float,
    kR16G16B16A16_float,
    kR32G32B32_float,
    kR32G32B32A32_float,
  };

}
namespace DR_TEXTURE_ADDRESS {
  enum E
  {
    kTiled,
    kClampToEdge,
    kMirror
  };
}

namespace DR_TEXTURE_FILTER {
  enum E {
    kPoint,
    kLinear,
    kAnisotropic
  };
}

namespace DR_PRIMITIVE_TOPOLOGY {
  enum E {
    kLines,
    kTriangles,
    kQuads
  };
}

namespace DR_GRAPHICS_ERROR {
  enum E {
    ERROR_NONE = 0,
    COMPILE_SHADER_ERROR = -1,
    CREATE_SHADER_ERROR  = -2,
    CREATE_BUFFER_ERROR  = -3,
  };
}

namespace DR_DEPTH_STENCIL_CLEAR_TYPE {
enum E {
  kClearDepth,
  kClearStencil,
  kClearDepthStencil
};
}

namespace DR_SHADER_TYPE_FLAG {
enum E {
  kVertex = 1,
  kFragment = 1 << 1,
  kHull = 1 << 2,
  kCompute = 1 << 3,
  kTexture = 1 << 4,
  kTeselation = 1 << 5,
  kDomain = 1 << 6,
  kGeometry = 1 << 8
};
}

struct DR_API_EXPORT DrInputDesc
{
  UInt32 semanticIndex;
  DR_FORMAT::E format;
  UInt32 offset;
};

struct DR_API_EXPORT DrSampleDesc
{
  DR_TEXTURE_FILTER::E Filter;
  DR_TEXTURE_ADDRESS::E addressU;
  DR_TEXTURE_ADDRESS::E addressV;
  DR_TEXTURE_ADDRESS::E addressW;
};

struct DR_API_EXPORT DrBufferDesc
{
  DR_BUFFER_USAGE::E usage;
  uint32_t stride;
  uint32_t sizeInBytes;
};

}