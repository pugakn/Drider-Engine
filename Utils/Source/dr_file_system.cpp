#include "dr_file_system.h"
#include "dr_string_utils.h"

namespace driderSDK
{

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

bool
FileSystem::CreateAndOpen(const String& filename, File& file) {
  std::ofstream newFile(filename);
  newFile.close();

  return file.Open(filename);
}

void
FileSystem::Copy(const String& filepathSrc, const String& filepathDst) {
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
FileSystem::Move(const String& filepathSrc, const String& filepathDst) {
  Copy(filepathSrc, filepathDst);
  Remove(filepathSrc);
}

bool
FileSystem::Remove(const String& filepath) {
  String rename = StringUtils::toString(filepath);

  bool successfullyErased = remove(rename.c_str()) == 0;

  return successfullyErased;
}

bool
FileSystem::IsFile(const String& filepath) {
  String rename = StringUtils::toString(filepath);

  struct stat s;

  if (stat(rename.c_str(), &s) == 0) {
    if (s.st_mode & S_IFREG) {
      return true;
    }
  }

  return false;
}

bool
FileSystem::IsDirectory(const String& filepath) {
  String rename = StringUtils::toString(filepath);

  struct stat s;

  if (stat(rename.c_str(), &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      return true;
    }
  }

  return false;
}

bool
FileSystem::Exists(const String& filepath) {
  std::ifstream f(filepath.c_str());
  return f.good();
}

String
FileSystem::GetFileExtension(const String& file) {
  SizeT pos = file.find_last_of(_T('.'));

  String extension;

  if (pos != String::npos) {
    extension = file.substr(pos + 1);
  }

  return extension;
}

}