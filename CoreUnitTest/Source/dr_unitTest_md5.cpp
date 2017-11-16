#include <dr_md5.h>

#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>


TEST(MD5, md5Encrypting) {
  std::string message = "";
  std::string md5Message = "";

  message = "Triangle";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "5e5500cb2b82eb72d550de644bd1b64b");

  message = "Eye";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "ae160b4691f9e79ba0f78369237218d0");

  message = "YoStarduck!";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "4f6739eea22a3751eb385501e6ad136a");

  message = "Is YoStarduck illuminati confirmed?";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "d69d9723aca2a946e384815b867e51cc");

  message = "Evil 666 Test. WTF?!";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "3721b0ead33d8c9e67185bc225d93157");

  message = "The most merciful thing in the world, i think, is the inability of the human mind to correlate all its contents.";
  md5Message = driderSDK::MD5(message);
  EXPECT_EQ(md5Message, "92f54a2ae1acf15c6716a979fc80387c");
}