#pragma once
#include "dr_core_prerequisites.h"
#include "dr_file.h"

namespace driderSDK {

/**
* Class to load files in buffers
*
* Sample usage:
* FileSystem sampleName;
*/
class DR_CORE_EXPORT FileSystem
{
 public:
  
  /**
  * TEST::defaultConstructor
  *
  * Default FileSystem constructor
  */
   FileSystem();

  /**
  * TEST::defaultDestructor
  *
  * Default FileSystem constructor
  */
  ~FileSystem();
  
  /**
  * TEST::GetWorkingPath
  *
  * Returns the fullpath of the working directory.
  * OS specific.
  *
  * @return
  *   String containing the fullpath of the working directory.
  */
  TString
  GetWorkingPath();

  /**
  * TEST::CreateAndOpen
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  bool
  CreateAndOpen(const String& filename, File& file);

  /**
  * TEST::Copy
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  void
  Copy(const String& filepathSrc, const String& filepathDst);

  /**
  * TEST::Move
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  void
  Move(const String& filepathSrc, const String& filepathDst);

  /**
  * TEST::Remove
  *
  * Description
  *
  * @param filename
  *
  * @return
  *   
  */
  bool
  Remove(const String& filepath);

  /**
  * TEST::IsFile
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  bool
  IsFile(const String& filepath);

  /**
  * TEST::IsDirectory
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  bool
  IsDirectory(const String& filepath);

  /**
  * TEST::Exists
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  bool
  Exists(const String& filepath);

  /**
  * TEST::GetFileExtension
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  static String 
  GetFileExtension(const String& filepath);

};

}