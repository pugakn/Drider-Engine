#include <dr_file.h>
#include <gtest\gtest.h>
#include <dr_string_utils.h>

TEST(File, defaultConstructor89) {
  driderSDK::File* someFile = new driderSDK::File;

  EXPECT_FALSE(nullptr == someFile);

  delete someFile;
}

TEST(File, defaultDestructor) {
  driderSDK::File someFile;

  EXPECT_TRUE(true);
}

TEST(File, Open) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  someFile.Close();

  EXPECT_TRUE(fileOpened);
}

TEST(File, Close) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  someFile.Close();

  EXPECT_TRUE(fileOpened);
}

TEST(File, Seek) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  someFile.Seek(1);
  someFile.Close();

  EXPECT_TRUE(fileOpened);
}

TEST(File, TellG) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  someFile.Seek(1);
  driderSDK::SizeT filePtr = someFile.TellG();
  someFile.Close();

  EXPECT_TRUE(filePtr == 1);
}

TEST(File, Size) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  driderSDK::SizeT fileSz = someFile.Size();
  someFile.Close();

  EXPECT_TRUE(fileSz == 21);
}

TEST(File, Read) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoSrc.txt");
  driderSDK::ANSIChar* heartContainer = new driderSDK::ANSIChar[2];
  someFile.Seek(19);
  someFile.Read(2, heartContainer);
  someFile.Close();

  bool hcLessT = heartContainer[0] == '<';
  bool hcThree = heartContainer[1] == '3';

  delete[] heartContainer;

  EXPECT_TRUE(hcLessT && hcThree);
}

TEST(File, Write) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoDst.txt");
  driderSDK::ANSIChar c[] = "Cmamo Cmamut Cmarnat Cva a ir a otro pais sin mi";
  someFile.Write(48, c);
  driderSDK::SizeT fileSz = someFile.Size();
  someFile.Close();

  EXPECT_TRUE(fileSz == 48);
}

TEST(File, GetAsString) {
  driderSDK::File someFile;

  bool fileOpened = someFile.Open(L"PatoDst.txt");
  driderSDK::TString miDrepesionPuntoTXT = someFile.GetAsString(someFile.Size());
  someFile.Close();

  EXPECT_TRUE(miDrepesionPuntoTXT.size() >= 48);
}