#include "dr_file_system.h"
#include "dr_string_utils.h"

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

  return file.Open(filename);
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
  bool successfullyErased = remove(StringUtils::toString(filepath).c_str()) == 0;

  return successfullyErased;
}

bool
FileSystem::IsFile(const TString& filepath) {
  struct stat s;

  if (stat(StringUtils::toString(filepath).c_str(), &s) == 0) {
    if (s.st_mode & S_IFREG) {
      return true;
    }
  }

  return false;
}

bool
FileSystem::IsDirectory(const TString& filepath) {
  TString rename = filepath;

  struct stat s;

  if (stat(StringUtils::toString(rename).c_str(), &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      return true;
    }
  }

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

TString
FileSystem::GetFileName(const TString& filepath) {
  SizeT slashPos = filepath.find_last_of(_T('\\'));
  SizeT dotPos = filepath.find_last_of(_T('.'));

  TString name;

  if (slashPos != TString::npos) {
    name = filepath.substr(slashPos - 1, dotPos - slashPos);
  } 
  else {
    name = filepath.substr(0, dotPos - 1);
  }

  return name;
}

}