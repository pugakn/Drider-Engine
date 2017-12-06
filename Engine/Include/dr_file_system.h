#pragma once
#include "dr_engine_prerequisites.h"
#include "dr_file.h"

namespace driderSDK {

/**
* Class to load files in buffers
*
* Sample usage:
* FileSystem sampleName;
*/
class DR_ENGINE_EXPORT FileSystem
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
  CreateAndOpen(const TString& filename, File& file);

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
  Copy(const TString& filepathSrc, const TString& filepathDst);

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
  Move(const TString& filepathSrc, const TString& filepathDst);

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
  Remove(const TString& filepath);

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
  IsFile(const TString& filepath);

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
  IsDirectory(const TString& filepath);

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
  Exists(const TString& filepath);

  /**
  * TEST::GetFileExtension
  *
  * Description
  *
  * @param filename
  *
  * @return
  */
  static TString 
  GetFileExtension(const TString& filepath);

};

}