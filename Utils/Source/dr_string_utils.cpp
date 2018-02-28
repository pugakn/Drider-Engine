#include "dr_string_utils.h"
#include <locale>
#include <codecvt>

namespace driderSDK {

String 
StringUtils::toString(const WString& wstring) {
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().to_bytes(wstring);
}

WString 
StringUtils::toWString(const String& string) {
  return std::wstring_convert<std::codecvt_utf8_utf16<WChar>>().from_bytes(string);
}

TString 
StringUtils::toTString(const String& string) {
#ifdef UNICODE
  return toWString(string);
#else
  return string;
#endif
}

TString 
StringUtils::toTString(const WString& wstring) {
#ifdef UNICODE
  return wstring;
#else
  return toString(wstring);
#endif
}
}