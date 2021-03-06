#include "dr_file_system.h"
#if DR_PLATFORM == DR_PLATFORM_WINDOWS
#include <direct.h>

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

}

#endif