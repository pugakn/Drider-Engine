#include "dr_file_system.h"

namespace driderSDK
{

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

bool
FileSystem::CreateAndOpen(const TString& filename, File& file) {
  std::ofstream newFile(filename);
  newFile.close();

  file.Open(filename);

  return false;
}

void
FileSystem::Copy(const TString& filepathSrc, const TString& filepathDst) {
  File srcFile;
  srcFile.Open(filepathSrc);

  File dstFile;
  CreateAndOpen(filepathDst, dstFile);

  SizeT srcSz = srcFile.Size();
  ANSIChar* srcFileBytes = new ANSIChar[srcSz];
  srcFile.Read(srcSz, &srcFileBytes[0]);
  srcFile.Close();

  dstFile.Write(srcSz, &srcFileBytes[0]);
  dstFile.Close();

  delete[] srcFileBytes;
}

void
FileSystem::Move(const TString& filepathSrc, const TString& filepathDst) {
  Copy(filepathSrc, filepathDst);
  Remove(filepathSrc);
}

bool
FileSystem::Remove(const TString& filepath) {
  SizeT filepathLenght = filepath.length();
  char* rename = new char[filepathLenght];
  std::wcstombs(rename, filepath.c_str(), filepathLenght);

  bool successfullyErased = remove(rename) != 0;

  delete[] rename;
  return successfullyErased;
}

bool
FileSystem::IsFile(const TString& filepath) {
  SizeT filepathLenght = filepath.length();
  char* rename = new char[filepathLenght];
  std::wcstombs(rename, filepath.c_str(), filepathLenght);

  struct stat s;

  if (stat(rename, &s) == 0) {
    if (s.st_mode & S_IFREG) {
      delete[] rename;
      return true;
    }
  }

  delete[] rename;
  return false;
}

bool
FileSystem::IsDirectory(const TString& filepath) {
  SizeT filepathLenght = filepath.length();
  char* rename = new char[filepathLenght];
  std::wcstombs(rename, filepath.c_str(), filepathLenght);

  struct stat s;

  if (stat(rename, &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      delete[] rename;
      return true;
    }
  }

  delete[] rename;
  return false;
}

bool
FileSystem::Exists(const TString& filepath) {
  std::ifstream f(filepath.c_str());
  return f.good();
}

TString
FileSystem::GetFileExtension(const TString& file) {
  SizeT pos = file.find_last_of(_T('.'));

  TString extension;

  if (pos != TString::npos) {
    extension = file.substr(pos + 1);
  }

  return extension;
}

}