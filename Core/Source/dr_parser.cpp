#include "dr_parser.h"
#include <locale>
#include <codecvt>
#include <string>

namespace driderSDK {

String Parser::toUTF8(const TString& str) {
#ifdef _UNICODE
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().to_bytes(str);
#else
  return str;
#endif
}

WString Parser::toUTF16(const TString& str) {
#ifdef _UNICODE
  return str;
#else
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().from_bytes(str);
#endif
}

std::vector<TString>
Parser::split(TString str,
              const TString& separators, 
              bool keepEmptyTokens) {

  DR_ASSERT(!separators.empty());

  std::vector<TString> tokens;

  SizeT pos = 0;
  
  while( (pos = str.find_first_of(separators)) != TString::npos )
  {
    TString token = str.substr(0, pos++);

    if (!token.empty() || keepEmptyTokens) {
      tokens.push_back(std::move(token));
    }

    str.erase(str.begin(), str.begin() + pos);
  }

  if(!str.empty() || keepEmptyTokens) {
    tokens.push_back(str);
  }
  
  return std::move(tokens);;
}

}