#include <dr_file_system.h>
#include <gtest\gtest.h>

TEST(FileSystem, defaultConstructor) {
  driderSDK::FileSystem* ptrFS = new driderSDK::FileSystem();

  EXPECT_FALSE(nullptr == ptrFS);

  delete ptrFS;
}

TEST(FileSystem, defaultDestructor) {
  driderSDK::FileSystem* fsTest = new driderSDK::FileSystem();

  delete fsTest;
  fsTest = nullptr;

  EXPECT_FALSE(fsTest);
}

TEST(FileSystem, load) {
  driderSDK::FileSystem fsTest;

  std::string bufferOutput;
  bool fileOpened = fsTest.load("UnexistingFile1.txt", bufferOutput);

  EXPECT_FALSE(fileOpened);
}

TEST(FileSystem, save) {
  driderSDK::FileSystem fsTest;

  std::string savedBuffer = "IMPOSIBLETOSAVE";
  bool fileOpened = fsTest.save("UnexistingFile2.txt", savedBuffer);

  EXPECT_TRUE(fileOpened);
}

TEST(FileSystem, append) {
  driderSDK::FileSystem fsTest;

  std::string addedBuffer;
  bool fileOpened = fsTest.append("UnexistingFile3.txt", addedBuffer);

  EXPECT_TRUE(fileOpened);
}