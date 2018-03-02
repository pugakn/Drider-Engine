#include "dr_file_system.h"
#if DR_PLATFORM == DR_PLATFORM_LINUX
#include <unistd.h>

namespace driderSDK
{

String
FileSystem::GetWorkingPath() {
  String fullpath;
  ANSIChar cCurrentPath[FILENAME_MAX];

  if (!getcwd(cCurrentPath, sizeof(cCurrentPath))) {
    return fullpath;
  }

  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

  SizeT curChar = 0;
  while (curChar < sizeof(cCurrentPath)) {
    fullpath += cCurrentPath[curChar];
    ++curChar;
  }

  return fullpath;
}

}

#endif