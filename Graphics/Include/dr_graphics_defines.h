#pragma once
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
  kAplha,
  kR4G4B4,
  kR4G4B4A4,
  kR8G8B8,
  kR8G8B8A8,
  kR16G16B16,
  kR16G16B16A16,
  kR32G32B32,
  kR32G32B32A32
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

struct DrSampleDesc 
{
  DR_TEXTURE_FILTER::E Filter;
  DR_TEXTURE_ADDRESS::E addressU;
  DR_TEXTURE_ADDRESS::E addressV;
  DR_TEXTURE_ADDRESS::E addressW;
};


