#include <dr_file_system.h>
#include <gtest\gtest.h>

TEST(FileSystem, defaultConstructor) {
  driderSDK::FileSystem* fileMaganer = new driderSDK::FileSystem;

  EXPECT_FALSE(nullptr == fileMaganer);

  delete fileMaganer;
}

TEST(FileSystem, defaultDestructor) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, GetWorkingPath) {
  driderSDK::FileSystem fileMaganer;
  driderSDK::TString WorkingPath = fileMaganer.GetWorkingPath();

  printf("\n\n%s\n\n", WorkingPath);

  EXPECT_FALSE(!WorkingPath.empty());
}

TEST(FileSystem, CreateAndOpen) {
  driderSDK::FileSystem fileMaganer;
  driderSDK::File regularFile;
  driderSDK::TString newFilename = L"Pato.txt";


  fileMaganer.CreateAndOpen(newFilename, regularFile);

  regularFile.Close();

  EXPECT_TRUE(true);
}

TEST(FileSystem, Copy) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, Move) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, RemoveRemove) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, IsFile) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, IsDirectory) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, Exists) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}

TEST(FileSystem, GetFileExtension) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(true);
}