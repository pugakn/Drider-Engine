#include <dr_fileSystem.h>
#include <gtest\gtest.h>

TEST(FileSystem, defaultConstructor) {
  driderSDK::FileSystem* ptrFS = nullptr;
  ptrFS = new driderSDK::FileSystem();

  EXPECT_FALSE(nullptr == ptrFS);

  delete ptrFS;
}

TEST(FileSystem, defaultDestructor) {
  driderSDK::FileSystem* fsTest = nullptr;
  fsTest = new driderSDK::FileSystem();

  delete fsTest;
  fsTest = nullptr;

  EXPECT_TRUE(nullptr == fsTest);
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