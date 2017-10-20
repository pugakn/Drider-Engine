#include "dr_parser.h"

namespace driderSDK {

const char*
Parser::strStr(const char* str1,
               const char* str2) {
  return strstr(str1, str2);   
}

int
Parser::strNCpy(char* destination,
                const char * source,
                size_t size) {
  return strcpy_s(destination, size, source);

}

char*
Parser::strCpyBetween(const char* source,
              const char* strFrom,
              const char* strTo,
              char& destinationOut) {
  return NULL; // No implemented
}

}