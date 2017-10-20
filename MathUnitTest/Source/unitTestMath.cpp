#include <gtest\gtest.h>
#include <dr_logger.h>

extern driderSDK::Module<driderSDK::Logger> ModuleLogger;
/*
void yourWaifuIsShit() {
  driderSDK::Logger* theNameIsATruth = ModuleLogger.instancePtr();
  theNameIsATruth->addWarning(__FILE__, __LINE__, "I Am Error");
}
*/

int main(int argc, char* argv[])
{
  driderSDK::Logger* mainLogger = ModuleLogger.instancePtr();
  mainLogger->reset();
  mainLogger->addWarning(__FILE__, __LINE__, "A warning sample");

  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "Vector4D.*";
  //::testing::GTEST_FLAG(filter) = "Radian.*";
  //::testing::GTEST_FLAG(filter) = "Vector3D.*";
  RUN_ALL_TESTS();

  return 0;
}