#include "dr_parser.h"

namespace driderSDK {

TString
Parser::strStr(TString str1,
               TString str2) {
 #ifdef _UNICODE
  return wcsstr(str1.c_str(), str2.c_str());
 #else
  return strstr(str1.c_str(), str2.c_str());
 #endif // _UNICODE
}

size_t
Parser::strLen(TString str) {
 #ifdef _UNICODE
  return wcslen(str.c_str());
 #else
  return strlen(str.c_str());
 #endif // _UNICODE

}

TString
Parser::strCpyBetween(TString source,
                      TString strFrom,
                      TString strTo,
                      TString &destinationOut) {

 TString first = Parser::strStr(source, strFrom);
 TString last = Parser::strStr(first, strTo);

 if (first != L"" && last != L"") {
  size_t offset = Parser::strLen(first.c_str()) - Parser::strLen(last.c_str());
  TString buffer;
  TString::iterator it = first.begin();

  size_t strSize = Parser::strLen(strFrom.c_str());
  offset -= strSize;
  while(strSize > 0) {
    strSize--;
    *it++;
  }
  
  while(offset-- > 0) {
    buffer.push_back(*it++);
  }
  destinationOut = buffer;
  return buffer;
 }
 return L"";
}

TString
Parser::addUntilFind(TString source,
                     TString delimiter,
                     TString& strOut) {
  TString buffer;
  TString::iterator it = source.begin();
  while(*&it[0] != delimiter[0]) {
    buffer.push_back(*it++);
  }
  strOut = buffer;
  return buffer;
}

std::vector<TString>
Parser::split(TString str,
      TString divider) {
 str.push_back(divider[0]);
 TString::iterator it = str.begin();
 
 size_t sizeD = driderSDK::Parser::strLen(divider);
 
 std::vector<TString> result;
 TString buffer;
 bool bAdd = true;

 while(it != str.end()) {
  bAdd = true;
  for(size_t i = 0; i < sizeD; i++) {
   if(*it == divider[i]) {
    bAdd = false;
   }
  }
  if(bAdd) {
   buffer += *it;
  } else {
   if(buffer.size() > 0) {
    result.push_back(buffer);
    buffer.clear();
   }
  }
  it++;
 }
 return result;
}

}