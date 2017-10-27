#include "dr_parser.h"

namespace driderSDK {

const char*
Parser::strStr(const char* str1,
               const char* str2) {
 return strstr(str1, str2);   
}

char*
Parser::strCpyBetween(const char* source,
                      const char* strFrom,
                      const char* strTo,
                      char*& destinationOut) {
 const char * pChar = Parser::strStr(source, strFrom);
 pChar+= sizeof(strFrom) / 2;

 const char * pLastChar = Parser::strStr(source, strTo);

 size_t newSize = (strlen(pChar) / sizeof(char)) - (strlen(pLastChar) / sizeof(char));

 char* buffer = new char[newSize + 1]{};

 if(*pChar != NULL && *pLastChar != NULL) {
  Int16 index = 0;
  while((char*)pChar != (char*)pLastChar) {
   buffer[index++] = *pChar;
   pChar++;
  }
 } else {
  return NULL;
 }
 
 destinationOut = &buffer[0];
 return buffer;
}

bool
Parser::compare(const char* str1,
                const char* str2) {

 std::string s1 = str1;
 std::string s2 = str2;
 
 return s1.compare(s2) == 0;
}

char *
Parser::addUntilFind(const char* source,
                     const char* delimiter,
                     char *& strOut) {

 size_t sizeSource = strlen(source) / sizeof(char);
 size_t sizeDelimiter = strlen(delimiter) / sizeof(char);

 char* str = (char*)source;
 char* result = new char[sizeSource] {};
 
 for(size_t i = 0; i < sizeSource; i++) {
  bool use = true;
  
  for(size_t j = 0; j < sizeDelimiter; j++) {
   if(str[i] == delimiter[j]) {
     use = false;
   }
  }

  if(use) {
    result[i] += str[i];
  } else {
    strOut = result;
    return result;
  }
 }
 strOut = result;
 return result;
}

std::vector<std::string>
Parser::split(char* str,
              const char* divider) {

 std::vector<std::string> result;
 
 size_t len = strlen(str);

 char* ret = new char[len + 2];
 char *r = ret;
 ret[10] = 'a';
 Parser::strCopy(ret, str);
 ret[len] = divider[0];
 ret[len + 1] = '\0';

 while(Parser::compare(ret, "\0") != true) {
  char* nString = "";
  Parser::addUntilFind(ret, divider, nString);
  ret += strlen(nString) / sizeof(char) + 1;
  if(!Parser::compare(nString, "") && !Parser::compare(nString, divider)) {
   result.push_back(nString);
  }
 }
 delete[] r;
 return result;
}

char *
Parser::strCopy(char* des,
               const char* source) {
  return strcpy(des, source);
}

}