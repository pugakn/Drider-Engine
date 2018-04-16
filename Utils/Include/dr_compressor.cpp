//#include "dr_compressor.h"
//
//#include <LZ4\lz4.h>
//namespace driderSDK {
//Int32 
//Compressor::compress(const byte * src, byte * dst, int srcSize, int dstCapacity, int acceleration)
//{
//  return LZ4_compress_fast((char*)src,  (char*)dst, srcSize, dstCapacity, acceleration);
//}
//
//Int32
//Compressor::decompress(const byte * src, byte * dst, int compressedSize, int dstCapacity)
//{
//  return LZ4_decompress_safe((char*)src, (char*)dst, compressedSize, dstCapacity);
//}
//}