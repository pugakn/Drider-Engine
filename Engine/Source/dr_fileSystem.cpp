#include "dr_fileSystem.h"

namespace driderSDK
{

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

bool
FileSystem::load(const std::string filename,
                 std::string& bufferOut) {
  std::ifstream fileLoaded;

  fileLoaded.open(filename);
  if (fileLoaded.is_open())
  {
    bufferOut = std::string(std::istreambuf_iterator<char>(fileLoaded),
                            std::istreambuf_iterator<char>());
    fileLoaded.close();
    return true;
  }
  else
  {
    fileLoaded.close();
    return false;
  }
}

bool
FileSystem::save(const std::string filename,
                 const std::string& bufferToSave) {
  std::ofstream fileLoaded;

  fileLoaded.open(filename, std::ios::out | std::ios::trunc);
  if (fileLoaded.is_open())
  {
    fileLoaded << bufferToSave;
    fileLoaded.close();
    return true;
  }
  else
  {
    fileLoaded.close();
    return false;
  }
}

bool
FileSystem::append(const std::string filename,
                   const std::string& bufferToAppend) {
  std::ofstream fileLoaded;

  fileLoaded.open(filename, std::ios::out | std::ios::app);
  if (fileLoaded.is_open())
  {
    fileLoaded << bufferToAppend;
    fileLoaded.close();
    return true;
  }
  else
  {
    fileLoaded.close();
    return false;
  }
}

}