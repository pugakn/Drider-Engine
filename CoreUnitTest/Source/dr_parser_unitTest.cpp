#include <dr_parser.h>
#include <dr_core_prerequisites.h>

#include <gtest\gtest.h>

TEST (Parser, parserStrstr) {
  driderSDK::TString str1 = _T("hola mundo como estan");
  driderSDK::TString str2 = driderSDK::Parser::strStr(str1, _T("mundo"));
  EXPECT_TRUE(str1.compare(_T("mundo como estan")));
}

TEST(Parser, parserStrCpyBetween) {
  driderSDK::TString r;
  driderSDK::TString s;
  r = driderSDK::Parser::strCpyBetween(_T("hola mundo, como estan"),
                                       _T("mundo"),
                                       _T("estan"),
                                       s);
  EXPECT_TRUE(r == s);
  EXPECT_TRUE(r == _T(", como "));
}

TEST(Parser, parserStringToInt32) {
  EXPECT_TRUE(driderSDK::Parser::stringToInt32(_T("10")) == 10);
  EXPECT_TRUE(driderSDK::Parser::stringToInt32(_T("10.1")) == 10);
}

TEST(Parser, parserStringToFloat) {
  EXPECT_TRUE(driderSDK::Parser::stringToFloat(_T("10")) == 10.0f);
  EXPECT_TRUE(driderSDK::Parser::stringToFloat(_T("10.65")) == 10.65f);
}

TEST (Parser, parserAddUntilFind) {
  driderSDK::TString out;
  driderSDK::Parser::addUntilFind(_T("1.0, 25.0, 236"), _T(","), out);
  EXPECT_TRUE(out == _T("1.0"));
}

TEST (Parser, parserSplit) {
  std::vector<driderSDK::TString> result;
  std::vector<driderSDK::TString> compare { _T("azul"), _T("rojo"), _T("verde"), _T("amarillo")};
  result = driderSDK::Parser::split(_T("azul, rojo,verde, amarillo"), _T(", "));
  EXPECT_TRUE(result == compare);
}