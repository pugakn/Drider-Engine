#include "dr_file_system.h"
#if DR_PLATFORM == DR_PLATFORM_WINDOWS
#include <direct.h>
#include <dr_string_utils.h>
#include <windows.h>
#include <dr_logger.h>

namespace driderSDK
{

TString
FileSystem::GetWorkingPath() {
  TString fullpath;
  ANSIChar cCurrentPath[FILENAME_MAX];

  if (!_getcwd(cCurrentPath, sizeof(cCurrentPath))) {
    return fullpath;
  }

  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

  SizeT curChar = 0;
  while (cCurrentPath[curChar] != '\0') {
    fullpath += cCurrentPath[curChar];
    ++curChar;
  }

  return fullpath;
}

std::vector<TString>
FileSystem::GetDirectoryContent(TString path) {
  std::vector<TString> content;

  if (path.empty()) {
    path = GetWorkingPath();
  }

  if (path.back() != '\\') {
    path += _T('\\');
  }
  path += _T("*.*");

  WIN32_FIND_DATA ffd;

  HANDLE hFind = FindFirstFile(StringUtils::toWString(path).c_str(), &ffd);
  if (hFind == INVALID_HANDLE_VALUE) {
    Logger::addError(__FILE__, __LINE__, _T("Invalid Path. ") + GetLastError());
    return content;
  }

  bool stillContent = true;
  while (stillContent) {
    content.push_back(ffd.cFileName);

    stillContent = FindNextFile(hFind, &ffd);
  }

  return content;
}

}

#endif