#include <dr_file_system.h>
#include <gtest\gtest.h>
#include <dr_string_utils.h>

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

  printf("\n");
  wprintf(WorkingPath.c_str());
  printf("\n\n");

  EXPECT_TRUE(!WorkingPath.empty());
}

TEST(FileSystem, CreateAndOpen) {
  driderSDK::FileSystem fileMaganer;

  driderSDK::File someRandomFile;
  
  bool fOpen = fileMaganer.CreateAndOpen(L"Pato.txt", someRandomFile);

  someRandomFile.Close();

  EXPECT_TRUE(fOpen);
}

TEST(FileSystem, Copy) {
  driderSDK::FileSystem fileMaganer;
  fileMaganer.Copy(L"Pato.txt", L"Pato2.txt");

  EXPECT_TRUE(true);
}

TEST(FileSystem, Move) {
  driderSDK::FileSystem fileMaganer;
  fileMaganer.Move(L"Pato2.txt", L"..\\Pato2.txt");

  EXPECT_TRUE(true);
}

TEST(FileSystem, Remove) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(fileMaganer.Remove(L"..\\Pato2.txt"));
}

TEST(FileSystem, IsFile) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(fileMaganer.IsFile(L"Pato.txt"));
}

TEST(FileSystem, IsDirectory) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(fileMaganer.IsDirectory(fileMaganer.GetWorkingPath()));
}

TEST(FileSystem, Exists) {
  driderSDK::FileSystem fileMaganer;

  EXPECT_TRUE(fileMaganer.Exists(L"Pato.txt"));
}

TEST(FileSystem, GetFileExtension) {
  driderSDK::FileSystem fileMaganer;
  driderSDK::TString fileExt = fileMaganer.GetFileExtension(L"Pato.txt");

  EXPECT_TRUE(fileExt == L"txt");
}