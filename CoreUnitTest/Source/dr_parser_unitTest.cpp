#include <dr_parser.h>
#include <dr_prerequisites.h>

#include <gtest\gtest.h>

TEST (Parser, parserStrstr) {
  driderSDK::TString str1 = L"hola mundo como estan";
  driderSDK::TString str2 = driderSDK::Parser::strStr(str1, L"mundo");
  EXPECT_TRUE(str1.compare(L"mundo como estan"));
}

TEST(Parser, parserStrCpyBetween) {
  driderSDK::TString r;
  driderSDK::TString s;
  r = driderSDK::Parser::strCpyBetween(L"hola mundo, como estan",
                                       L"mundo",
                                       L"estan",
                                       s);
  EXPECT_TRUE(r == s);
  EXPECT_TRUE(r == L", como ");
}

TEST(Parser, parserStringToInt32) {
  EXPECT_TRUE(driderSDK::Parser::stringToInt32(L"10") == 10);
  EXPECT_TRUE(driderSDK::Parser::stringToInt32(L"10.1") == 10);
}

TEST(Parser, parserStringToFloat) {
  EXPECT_TRUE(driderSDK::Parser::stringToFloat(L"10") == 10.0f);
  EXPECT_TRUE(driderSDK::Parser::stringToFloat(L"10.65") == 10.65f);
}

TEST (Parser, parserAddUntilFind) {
  driderSDK::TString out;
  driderSDK::Parser::addUntilFind(L"1.0, 25.0, 236", L",", out);
}

TEST (Parser, parserSplit) {

}