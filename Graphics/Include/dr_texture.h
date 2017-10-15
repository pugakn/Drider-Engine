#pragma once
#include <dr_prerequisites.h>
#include <string>
namespace driderSDK {
namespace DR_TEXTURE_FORMAT {
enum E {
  kAplha,
  kRGB4,
  kRGBA4,
  kRGB16,
  kRGBA16,
  kRGB32,
  kRGBA32
};
}
namespace DR_TEXTURE_PARAMS {
enum E {
  kTiled = 1,
  kClampToEdge = 2,
  kMirror = 4
};
}

class Texture {
 public:
  virtual void Load() = 0;
  virtual void Destroy() = 0;
   
  Int32 width;
  Int32 height;
  Int32 pitch;
  Int32 size;
  UInt32 flags;
  UInt32 id;
  DR_TEXTURE_FORMAT::E format;
  std::string name;
 private:
};
}