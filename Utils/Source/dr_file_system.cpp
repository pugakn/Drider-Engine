#include "dr_file_system.h"
#include "dr_string_utils.h"
#include <Dirent\dirent.h>

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

void
FileSystem::ScanDir(String paht) {
  DIR *dir;
  char buffer[PATH_MAX + 2];
  char *p = buffer;
  const char *src;
  char *end = &buffer[PATH_MAX];
  int ok;

  /* Copy directory name to buffer */
  src = paht.c_str();
  while (p < end  &&  *src != '\0') {
    *p++ = *src++;
  }
  *p = '\0';

  /* Open directory stream */
  dir = opendir(paht.c_str());
  if (dir != NULL) {
    struct dirent *ent;

    /* Print all files and directories within the directory */
    while ((ent = readdir(dir)) != NULL) {
      char *q = p;
      char c;

      /* Get final character of directory name */
      if (buffer < q) {
        c = q[-1];
      }
      else {
        c = ':';
      }

      /* Append directory separator if not already there */
      if (c != ':'  &&  c != '/'  &&  c != '\\') {
        *q++ = '/';
      }

      /* Append file name */
      src = ent->d_name;
      while (q < end  &&  *src != '\0') {
        *q++ = *src++;
      }
      *q = '\0';

      /* Decide what to do with the directory entry */
      switch (ent->d_type) {
      case DT_LNK:
      case DT_REG:
        /* Output file name with directory */
        printf("%s\n", buffer);
        break;

      case DT_DIR:
        /* Scan sub-directory recursively */
        if (strcmp(ent->d_name, ".") != 0
            && strcmp(ent->d_name, "..") != 0) {
          ScanDir(buffer);
        }
        break;

      default:
        /* Ignore device entries */
        /*NOP*/;
      }

    }

    closedir(dir);
    ok = 1;

  }
  else {
    /* Could not open directory */
    printf("Cannot open directory %s\n", paht.c_str());
    ok = 0;
  }
}

}