#pragma once
#include "dr_util_prerequisites.h"
namespace driderSDK {
struct DR_UTIL_EXPORT Compressor {
  /**
  * TEST::compress
  *
  * @param src
  *	 The source buffer
  *
  * @param dst
  *  The destination buffer
  *
  * @param dstCapacity
  *  The destination buffer capacity
  *
  * @param acceleration
  *  The acceleration factor, more acceleration means
  *  more compression speed, but less compression
  *
  * @return 
  *  Number of bytes of the compressed file
  */
  static Int32 
  compress(const byte* src, byte* dst, int srcSize, int dstCapacity, int acceleration = 1);

  /**
  * TEST::decompress
  *
  * @param src
  *	 The source buffer
  *
  * @param dst
  *  The destination buffer
  *
  * @param compressedSize
  *  The src buffer size
  *
  * @param dstCapacity
  *  The destination buffer capacity, it must be more or
  *  equal to the original buffer size
  *
  * @return
  *  Number of bytes of the uncompressed file
  */
  static Int32 
  decompress(const byte* src, byte* dst, int compressedSize, int dstCapacity);
};
}