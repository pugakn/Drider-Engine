#include "dr_file_system.h"

namespace driderSDK
{

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
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

TString FileSystem::GetFileExtension(const TString& file) {
  SizeT pos = file.find_last_of(_T('.'));

  TString extension;

  if (pos != TString::npos) {
    extension = file.substr(pos + 1);
  }

  return extension;
}

}