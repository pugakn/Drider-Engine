#include "dr_parser.h"

namespace driderSDK {

const char*
Parser::strStr(const char* str1,
               const char* str2) {
  return strstr(str1, str2);   
}

char *
Parser::strCpy(char* destination,
                const char * source) {
  return strcpy(destination, source);

}

char*
Parser::strCpyBetween(const char* source,
              const char* strFrom,
              const char* strTo,
              char& destinationOut) {
  NULL; // No implemented
}

}