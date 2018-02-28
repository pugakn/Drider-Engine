#include <gtest\gtest.h>
#include "dr_time.h"


using namespace driderSDK;

int main(int argc, char* argv[])
{

  Time::startUp();
  while(1) {
    Time::instance().update();
    std::cout << Time::getElapsed() <<  "\t"; 
    std::cout << Time::getElapsedMilli() << "\t";
    std::cout << Time::getElapsedMicro() << "\t";
    std::cout << Time::getDeltaMilli() << "\t" << std::endl;
  }

  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "ModelResource.*";
  //::testing::GTEST_FLAG(filter) = "FileSystem.*";
  //::testing::GTEST_FLAG(filter) = "File.*";
  RUN_ALL_TESTS();

  return 0;
}

