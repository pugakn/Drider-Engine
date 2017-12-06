#include "dr_file.h"

namespace driderSDK
{

File::File() {
}

File::~File() {
}

bool
File::Open(const TString filename) {
  m_file.open(filename, std::ios_base::out | std::ios_base::in | std::ios::binary);

  if (m_file) {
    m_filename = filename;

    return true;
  }
  else {
    m_file.close();
    return false;
  }
}

void
File::Close() {
  m_file.close();
  m_filename = TString();
}

void
File::Seek(const SizeT position) {
  assert(position <= Size());
  m_file.seekg(position, std::ios_base::beg);
}

SizeT
File::TellG() {
  return m_file.tellg();
}

SizeT
File::Size() {
  SizeT actualPtr = TellG();

  std::streampos fsize = 0;
  m_file.seekg(0, std::ios::beg);
  fsize = m_file.tellg();
  m_file.seekg(0, std::ios::end);
  fsize = m_file.tellg() - fsize;
  m_file.seekg(actualPtr, std::ios::beg);

  return fsize;
}

void
File::Read(const SizeT nBytes, ANSIChar* bytesOut) {
  SizeT filePtr = TellG();
  SizeT fileSz = Size();
  
  assert(filePtr + nBytes <= Size());
  m_file.read(bytesOut, nBytes);
}

void
File::Write(const SizeT nBytes, ANSIChar * bytesIn) {
  m_file.write(bytesIn, nBytes);
}

TString
File::GetAsString(const SizeT nBytes) {
  assert(TellG() + nBytes <= Size());

  ANSIChar* tmpBuff = new ANSIChar[nBytes];
  Read(nBytes, tmpBuff);

  TString resultingString;
  SizeT curIndex = 0;
  while (curIndex < nBytes) {
    resultingString += tmpBuff[curIndex];
    ++curIndex;
  }

  delete[] tmpBuff;

  return resultingString;
}

}