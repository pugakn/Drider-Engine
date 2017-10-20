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
  const char * pChar = Parser::strStr(source, strFrom);
  const char * pLastChar = Parser::strStr(source, strTo);
  char * buffer = "";

  if(*pChar != NULL && *pLastChar != NULL) {
   while(*pChar != *pLastChar) {
     buffer += *pChar;
     pChar++;
   }
  } else {
   return NULL;
  }
  
  destinationOut = *buffer;
  return buffer;
}

template <typename T> T
Parser::tryParse(T typeParse, const char *source) {
  return static_cast<T>(source);
}

template <typename T> char*
Parser::toString(T value) {
  char* r = "";
  return static_cast<char*>(*value);
}

}