#include "dr_md5.h"
#include <sstream>

namespace driderSDK {

std::string
MD5(std::string initial_msg) {
  //Integers part of the sines of integers (in radians) * 2^32.
  static const UInt32 integerSin[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
  };

  //Number of shifts to do per round.
  static const UInt32 shiftAmount[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
  };

  SizeT initial_len = initial_msg.size();
  Int32 new_len;
  for (new_len = initial_len * 8 + 1; new_len % 512 != 448; ++new_len);
  new_len /= 8;

  //Also appends "0" bits 
  //Allocing 64 extra bytes.
  UInt8 *msg = (UInt8*)std::calloc(new_len + 64, 1);

  std::memcpy(msg, initial_msg.c_str(), initial_len);

  //Write the "1" bit
  msg[initial_len] = 128;

  UInt32 bits_len = 8 * initial_len;        //Note: we append the len
  std::memcpy(msg + new_len, &bits_len, 4); //in bits at the end of the buffer.

  //Process the message in successive 512-bit chunks:
  //for each 512-bit chunk of message:
  UInt32 *w;
  UInt32 a, b, c, d, f, g;
  UInt32 rotX, rotC, temp;
  UInt32 hex[4] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
  for (Int32 offset = 0; offset < new_len; offset += 64) {
    //Break chunk into sixteen 32-bit words w[j], 0 <= j <= 15
    w = reinterpret_cast<UInt32 *>(msg + offset);

    //Initialize hash value for this chunk:
    a = hex[0];
    b = hex[1];
    c = hex[2];
    d = hex[3];

    //Hacking loop:
    for (Int32 i = 0; i < 64; ++i) {
      if (i < 16) {
        f = (b & c) | ((~b) & d);
        g = i;
      }
      else if (i < 32) {
        f = (d & b) | ((~d) & c);
        g = (5 * i + 1) % 16;
      }
      else if (i < 48) {
        f = b ^ c ^ d;
        g = (3 * i + 5) % 16;
      }
      else {
        f = c ^ (b | (~d));
        g = (7 * i) % 16;
      }

      temp = d;
      d = c;
      c = b;
      rotX = a + f + integerSin[i] + w[g];
      rotC = shiftAmount[i];
      b = b + ((rotX << rotC) | (rotX >> (32 - rotC)));
      a = temp;
    }

    //Add this chunk's hash to result.
    hex[0] += a;
    hex[1] += b;
    hex[2] += c;
    hex[3] += d;
  }

  //Cleanup
  free(msg);

  //Passing the result to a single string.
  std::string sResult = "";
  std::stringstream sstream;
  UInt8 *p;

  for (SizeT hexIndex = 0; hexIndex < 4; ++hexIndex) {
    p = reinterpret_cast<UInt8 *>(&hex[hexIndex]);
    for (SizeT hexOffset = 0; hexOffset < 4; ++hexOffset) {
      sstream.str("");
      sstream << std::hex << (p[hexOffset] << 4);
      sResult.append(sstream.str().substr(0, 2));
    }
  }

  return sResult;
}

}