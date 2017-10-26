#pragma once
#include <string>
#include <fstream>
#include <dr_defines.h>

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
class DR_API_EXPORT FileSystem
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
  * The Load operation determines how many bytes are in the file and returns the
  * amount in riSize.
  *
  * Param filename
  *   The name of the file to load.
  *
  * Param bufferOut
  *   The output where the file will be loaded
  *
  * Return
  *   Boolean value is true if the load is successful, in which case
  *   the outputs racBuffer and riSize are valid.
  *
  * If the returned value is false, one of the following conditions has occurred:
  *   The file does not exist.
  *   The file cannot be opened for reading (the file attributes might not allow this).
  */
  static bool
  load (const std::string filename, std::string& bufferOut);

  /*
  * The Save operation writes the input buffer to disk.
  *
  * Param filename
  *   The name of the file to save.
  *
  * Param bufferToSave
  *   The info to be saved.
  *
  * Return
  *   Boolean value is true if the save is successful.
  *   If the returned value is false, one of the following conditions has occurred:
  *     The input buffer is null.
  *     The file cannot be opened for writing (the file might exist and be set to read-only).
  */
  static bool
  save (const std::string filename, const std::string& bufferToSave);

  /*
  * The Append operation is identical in structure to Save, except that the file
  * is opened for appending.
  * The input buffer is written at the end of an already existing file. If the
  * file does not exist, it is created and the buffer is written.
  *
  * Param filename
  *   The name of the file to save.
  *
  * Param bufferToSave
  *   The info to be saved.
  *
  * Return
  *   Boolean value is true if the save is successful.
  *   If the returned value is false, one of the following conditions has occurred:
  *     The input buffer is null.
  *     The file cannot be opened for writing (the file might exist and be set to read-only).
  */
  static bool
  append (const std::string filename, const std::string& bufferToAppend);
};

}