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
  APLPHA,
  RGB4,
  RGBA4,
  RGB8,
  RGB8A,
  RGB16,
  RGBA16,
  RGB32,
  RGBA32
};

} 
namespace DR_TEXTURE_TILING {
enum E 
{
  TILED,
  CLAMP_TO_EDGE,
  MIRROR
};
}
