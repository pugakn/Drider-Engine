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
    kDrFormat_UNKNOWN = 0,
    kDrFormat_R32G32B32A32_TYPELESS = 1,
    kDrFormat_R32G32B32A32_FLOAT = 2,
    kDrFormat_R32G32B32A32_UINT = 3,
    kDrFormat_R32G32B32A32_SINT = 4,
    kDrFormat_R32G32B32_TYPELESS = 5,
    kDrFormat_R32G32B32_FLOAT = 6,
    kDrFormat_R32G32B32_UINT = 7,
    kDrFormat_R32G32B32_SINT = 8,
    kDrFormat_R16G16B16A16_TYPELESS = 9,
    kDrFormat_R16G16B16A16_FLOAT = 10,
    kDrFormat_R16G16B16A16_UNORM = 11,
    kDrFormat_R16G16B16A16_UINT = 12,
    kDrFormat_R16G16B16A16_SNORM = 13,
    kDrFormat_R16G16B16A16_SINT = 14,
    kDrFormat_R32G32_TYPELESS = 15,
    kDrFormat_R32G32_FLOAT = 16,
    kDrFormat_R32G32_UINT = 17,
    kDrFormat_R32G32_SINT = 18,
    kDrFormat_R32G8X24_TYPELESS = 19,
    kDrFormat_D32_FLOAT_S8X24_UINT = 20,
    kDrFormat_R32_FLOAT_X8X24_TYPELESS = 21,
    kDrFormat_X32_TYPELESS_G8X24_UINT = 22,
    kDrFormat_R10G10B10A2_TYPELESS = 23,
    kDrFormat_R10G10B10A2_UNORM = 24,
    kDrFormat_R10G10B10A2_UINT = 25,
    kDrFormat_R11G11B10_FLOAT = 26,
    kDrFormat_R8G8B8A8_TYPELESS = 27,
    kDrFormat_R8G8B8A8_UNORM = 28,
    kDrFormat_R8G8B8A8_UNORM_SRGB = 29,
    kDrFormat_R8G8B8A8_UINT = 30,
    kDrFormat_R8G8B8A8_SNORM = 31,
    kDrFormat_R8G8B8A8_SINT = 32,
    kDrFormat_R16G16_TYPELESS = 33,
    kDrFormat_R16G16_FLOAT = 34,
    kDrFormat_R16G16_UNORM = 35,
    kDrFormat_R16G16_UINT = 36,
    kDrFormat_R16G16_SNORM = 37,
    kDrFormat_R16G16_SINT = 38,
    kDrFormat_R32_TYPELESS = 39,
    kDrFormat_D32_FLOAT = 40,
    kDrFormat_R32_FLOAT = 41,
    kDrFormat_R32_UINT = 42,
    kDrFormat_R32_SINT = 43,
    kDrFormat_R24G8_TYPELESS = 44,
    kDrFormat_D24_UNORM_S8_UINT = 45,
    kDrFormat_R24_UNORM_X8_TYPELESS = 46,
    kDrFormat_X24_TYPELESS_G8_UINT = 47,
    kDrFormat_R8G8_TYPELESS = 48,
    kDrFormat_R8G8_UNORM = 49,
    kDrFormat_R8G8_UINT = 50,
    kDrFormat_R8G8_SNORM = 51,
    kDrFormat_R8G8_SINT = 52,
    kDrFormat_R16_TYPELESS = 53,
    kDrFormat_R16_FLOAT = 54,
    kDrFormat_D16_UNORM = 55,
    kDrFormat_R16_UNORM = 56,
    kDrFormat_R16_UINT = 57,
    kDrFormat_R16_SNORM = 58,
    kDrFormat_R16_SINT = 59,
    kDrFormat_R8_TYPELESS = 60,
    kDrFormat_R8_UNORM = 61,
    kDrFormat_R8_UINT = 62,
    kDrFormat_R8_SNORM = 63,
    kDrFormat_R8_SINT = 64,
    kDrFormat_A8_UNORM = 65,
    kDrFormat_R1_UNORM = 66,
    kDrFormat_R9G9B9E5_SHAREDEXP = 67,
    kDrFormat_R8G8_B8G8_UNORM = 68,
    kDrFormat_G8R8_G8B8_UNORM = 69,
    kDrFormat_BC1_TYPELESS = 70,
    kDrFormat_BC1_UNORM = 71,
    kDrFormat_BC1_UNORM_SRGB = 72,
    kDrFormat_BC2_TYPELESS = 73,
    kDrFormat_BC2_UNORM = 74,
    kDrFormat_BC2_UNORM_SRGB = 75,
    kDrFormat_BC3_TYPELESS = 76,
    kDrFormat_BC3_UNORM = 77,
    kDrFormat_BC3_UNORM_SRGB = 78,
    kDrFormat_BC4_TYPELESS = 79,
    kDrFormat_BC4_UNORM = 80,
    kDrFormat_BC4_SNORM = 81,
    kDrFormat_BC5_TYPELESS = 82,
    kDrFormat_BC5_UNORM = 83,
    kDrFormat_BC5_SNORM = 84,
    kDrFormat_B5G6R5_UNORM = 85,
    kDrFormat_B5G5R5A1_UNORM = 86,
    kDrFormat_B8G8R8A8_UNORM = 87,
    kDrFormat_B8G8R8X8_UNORM = 88,
    kDrFormat_R10G10B10_XR_BIAS_A2_UNORM = 89,
    kDrFormat_B8G8R8A8_TYPELESS = 90,
    kDrFormat_B8G8R8A8_UNORM_SRGB = 91,
    kDrFormat_B8G8R8X8_TYPELESS = 92,
    kDrFormat_B8G8R8X8_UNORM_SRGB = 93,
    kDrFormat_BC6H_TYPELESS = 94,
    kDrFormat_BC6H_UF16 = 95,
    kDrFormat_BC6H_SF16 = 96,
    kDrFormat_BC7_TYPELESS = 97,
    kDrFormat_BC7_UNORM = 98,
    kDrFormat_BC7_UNORM_SRGB = 99,
    kDrFormat_AYUV = 100,
    kDrFormat_Y410 = 101,
    kDrFormat_Y416 = 102,
    kDrFormat_NV12 = 103,
    kDrFormat_P010 = 104,
    kDrFormat_P016 = 105,
    kDrFormat_420_OPAQUE = 106,
    kDrFormat_YUY2 = 107,
    kDrFormat_Y210 = 108,
    kDrFormat_Y216 = 109,
    kDrFormat_NV11 = 110,
    kDrFormat_AI44 = 111,
    kDrFormat_IA44 = 112,
    kDrFormat_P8 = 113,
    kDrFormat_A8P8 = 114,
    kDrFormat_B4G4R4A4_UNORM = 115,

    kDrFormat_P208 = 130,
    kDrFormat_V208 = 131,
    kDrFormat_V408 = 132,


    kDrFormat_FORCE_UINT = 0xffffffff
  };

}
namespace DR_TEXTURE_ADDRESS {
  enum E
  {
    kWrap = 1,
    kMirror = 2,
    kClamp = 3,
    kBorder = 4,
    kMirrorOnce = 5
  };
}

namespace DR_TEXTURE_FILTER {
  enum E {
    kMIN_MAG_MIP_POINT = 0,
    kMIN_MAG_POINT_MIP_LINEAR = 0x1,
    kMIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
    kMIN_POINT_MAG_MIP_LINEAR = 0x5,
    kMIN_LINEAR_MAG_MIP_POINT = 0x10,
    kMIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
    kMIN_MAG_LINEAR_MIP_POINT = 0x14,
    kMIN_MAG_MIP_LINEAR = 0x15,
    kANISOTROPIC = 0x55,
    kCOMPARISON_MIN_MAG_MIP_POINT = 0x80,
    kCOMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
    kCOMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
    kCOMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
    kCOMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
    kCOMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    kCOMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
    kCOMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
    kCOMPARISON_ANISOTROPIC = 0xd5,
    kMINIMUM_MIN_MAG_MIP_POINT = 0x100,
    kMINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
    kMINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
    kMINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
    kMINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
    kMINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
    kMINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
    kMINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
    kMINIMUM_ANISOTROPIC = 0x155,
    kMAXIMUM_MIN_MAG_MIP_POINT = 0x180,
    kMAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
    kMAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
    kMAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
    kMAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
    kMAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
    kMAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
    kMAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
    kMAXIMUM_ANISOTROPIC = 0x1d5
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
    CREATE_SHADER_ERROR = -2,
    CREATE_BUFFER_ERROR = -3,
    ERROR_NOT_IMPLEMENTED = -4,
    CREATE_INPUT_LAYOUT_ERROR = -5,
    CREATE_RASTERIZER_STATE_ERROR = -6,
    CREATE_SAMPLER_STATE_ERROR = -7,
    CREATE_FACTORY_ERROR = -8,
    CREATE_SWAP_CHAIN_ERROR = -9,
    CREATE_TEXTURE_ERROR = -10,
    CREATE_RESOURCE_VIEW_ERROR = -11,
    MAP_RESOURCE_ERROR = -12,
    CREATE_DEPTH_STATE_ERROR = -13,
    CREATE_DEVICE_ERROR = -14
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
namespace DR_CPU_ACCESS_FLAG{
enum E {
  drWrite = 0x10000L,
  drRead  = 0x20000L
};
}

namespace DR_FILL_MODE {
enum E {
  kWireframe = 2,
  kSolid = 3
};
}

namespace DR_CULL_MODE {
enum E {
  kNone = 1,
  kFront = 2,
  kBack = 3
};
}

namespace DR_TEXTURE_TYPE {
enum E {
  dr1D,
  dr2D,
  dr3D
};
}

namespace DR_COMPARISON_FUNC {
enum E
{
  kNEVER = 1,
  kLESS = 2,
  kEQUAL = 3,
  kLESS_EQUAL = 4,
  kGREATER = 5,
  kNOT_EQUAL = 6,
  kGREATER_EQUAL = 7,
  kALWAYS = 8
};
}
namespace DR_SWAP_EFFECT {
enum E 
{
  kEFFECT_DISCARD = 0,
  kEFFECT_SEQUENTIAL = 1,
  kEFFECT_FLIP_SEQUENTIAL = 3,
  kEFFECT_FLIP_DISCARD = 4
};
}
struct DR_API_EXPORT DrInputElementDesc
{
  char*  semanticName;
  UInt32 semanticIndex;
  DR_FORMAT::E format;
  UInt32 inputSlot;
  UInt32 offset;
};

struct DR_API_EXPORT DrSampleDesc
{
  DR_TEXTURE_FILTER::E Filter;
  DR_TEXTURE_ADDRESS::E addressU;
  DR_TEXTURE_ADDRESS::E addressV;
  DR_TEXTURE_ADDRESS::E addressW;
  float mipLODBias;
  UInt32 maxAnisotropy;
  DR_COMPARISON_FUNC::E comparisonFunc;
  float borderColor[4];
  float minLOD;
  float maxLOD;
};

struct DR_API_EXPORT DrBufferDesc
{
  DR_BUFFER_USAGE::E usage;
  UInt32 stride;
  UInt32 sizeInBytes;
};

struct DR_API_EXPORT DrTextureDesc
{
  DR_FORMAT::E Format;
  DR_BUFFER_USAGE::E Usage;
  UInt32 width;
  UInt32 height;
  UInt32 pitch;
  UInt32 mipLevels;
  UInt32 CPUAccessFlags;
};

struct DR_API_EXPORT DrRasterizerDesc
{
  DR_FILL_MODE::E fillMode;
  DR_CULL_MODE::E cullMode;
  bool  frontCounterClockwise;
  Int32 depthBias;
  float depthBiasClamp;
  float slopeScaledDepthBias;
  bool depthClipEnable;
  bool scissorEnable;
  bool multisampleEnable;
  bool antialiasedLineEnable;
};
struct DR_API_EXPORT DrRationalNumber
{
  UInt32 numerator;
  UInt32 denominator;
};
struct DR_API_EXPORT DrSwapChainDesc 
{
  UInt32 width;
  UInt32 height;
  DrRationalNumber refreshRate;
  DR_FORMAT::E Format;
  UInt32 bufferCount;
  void* windowHandler;
  bool windowed;
};

struct DR_API_EXPORT DrDepthStencilDesc
{
  bool depthEnable;
  DR_COMPARISON_FUNC::E depthFunc; 
  bool stencilEnable;
  UInt8 stencilReadMask;
  UInt8 stencilWriteMask;
};
}