#pragma once
#include "dr_util_prerequisites.h"
namespace driderSDK {
struct DR_UTIL_EXPORT Compressor {
  static Int32 
  compress(const byte* src, byte* dst, int srcSize, int dstCapacity, int acceleration = 1);

  static Int32 
  decompress(const byte* src, byte* dst, int compressedSize, int dstCapacity);
};
}