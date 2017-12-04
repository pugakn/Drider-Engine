#pragma once
#include <string>
#include <fstream>
#include "dr_engine_prerequisites.h"

namespace driderSDK {

/**
* Class to load files in buffers
*
* Sample usage:
* FileSystem sampleName;
* sampleName.load("fileToOpen", bufferToStoreFile);
* sampleName.save("fileToWrite", bufferToSaveToFile);
* sampleName.append("fileToAppend", bufferToAddInFile);
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
  * The Load operation determines how many bytes are in the file and returns the
  * amount in riSize.
  *
  * @param filename
  *   The name of the file to load.
  *
  * @param bufferOut
  *   The output where the file will be loaded
  *
  * @return
  *   Boolean value is true if the load is successful, in which case
  *   the outputs racBuffer and riSize are valid.
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
  CreateAndOpen(const TString& filename);

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
  Copy(const TString& filepath);

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
  Move(const TString& filepath);

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
  GetFileExtension(const TString& file);

};

}