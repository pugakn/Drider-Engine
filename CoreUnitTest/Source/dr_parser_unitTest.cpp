#include <dr_parser.h>
#include <dr_core_prerequisites.h>

#include <gtest\gtest.h>


TEST(Parser, parserStringToInt32) {
  EXPECT_TRUE(driderSDK::Parser::stringToInt(_T("10")) == 10);
  EXPECT_TRUE(driderSDK::Parser::stringToInt(_T("10.19")) == 10);
}

TEST(Parser, parserStringToFloat) {
  EXPECT_TRUE(driderSDK::Parser::stringToReal(_T("10")) == 10.0f);
  EXPECT_TRUE(driderSDK::Parser::stringToReal(_T("10.65")) == 10.65f);
}

TEST(Parser, parserToString) {
  EXPECT_TRUE(driderSDK::Parser::toString(1.156f) == _T("1.156000"));
  EXPECT_TRUE(driderSDK::Parser::toString(1233) == _T("1233"));
  EXPECT_TRUE( driderSDK::Parser::toString(1.22) == _T("1.220000"));
  EXPECT_TRUE(driderSDK::Parser::toString(-2222) == _T("-2222"));
}

TEST (Parser, parserSplit) {
  std::vector<driderSDK::TString> result;
  std::vector<driderSDK::TString> compare { _T("azul"), _T("rojo"), _T("verde"), _T("amarillo")};
  result = driderSDK::Parser::split(_T("azul, rojo,verde, amarillo"), _T(", "));
  EXPECT_TRUE(result == compare);
}

