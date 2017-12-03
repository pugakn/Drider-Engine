#pragma once
#include <string>
#include <fstream>
#include "dr_engine_prerequisites.h"

namespace driderSDK {

/**
* Class to use files.
*
* Sample usage:
* File sampleName;
* sampleName.Open("fileToOpen.extension");
* sampleName.Write("fileToWrite");
* sampleName.Close("fileToAppend", bufferToAddInFile);
*/
class DR_ENGINE_EXPORT File
{
 public:
  
  /**
  * TEST::defaultConstructor
  *
  * Default File constructor
  */
   File();

  /**
  * TEST::defaultDestructor
  *
  * Default File constructor
  */
  ~File();

  /**
  * TEST::Open
  *
  * Tries to open a given file.
  * 
  * @param filename
  *  Name of the file to open
  *
  * @return
  *   If file was successfuly open or not.
  */
  bool
  Open(const TString filename);

  /**
  * TEST::Close
  *
  * Close the opened file.
  */
  void
  Close();

  /**
  * TEST::Seek
  *
  * Moves the file pointer to given absolute position.
  *
  * @param position
  *   new file pointer position.
  */
  void
  Seek(const SizeT position);

  /**
  * TEST::TellG
  *
  * Tells the file pointer position.
  *
  * @return
  *   The pointer position.
  */
  SizeT
  TellG();

  /**
  * TEST::Size
  *
  * Tells the size of file.
  *
  * @return
  *   Size in bytes of file.
  */
  SizeT
  Size();

  /**
  * TEST::Read
  *
  * Writes the number of bytes of file in
  * a buffer, starting in the current position.
  *
  * @param nBytes
  *   Number of bytes to read on file.
  *
  * @param bytesOut
  *   Buffer where the bytes are going to be writed.
  */
  void
  Read(const SizeT nBytes, ANSIChar* bytesOut);

  /**
  * TEST::GetAsString
  *
  * Return a string of the current position + nBytes
  *
  * @param nBytes
  *   Number of bytes to read as string.
  *
  * @return
  *   Resulting string.
  */
  TString
  GetAsString(const SizeT nBytes);

 protected:
  std::fstream m_file;
  TString m_filename;
};

}