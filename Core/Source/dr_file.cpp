#include "dr_file.h"

namespace driderSDK
{

File::File() : m_size(0)
{
}

File::~File() {
}

bool
File::Open(const String filename) {
  m_file.open(filename, std::ios::out | std::ios::in | std::ios::binary);

  if (m_file) {

    m_filename = filename;
    
    m_file.seekg(0, std::ios::end);

    m_size = static_cast<SizeT>(m_file.tellg());

    m_file.seekg(0, std::ios::beg);

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
  m_filename = String();
}

void
File::Seek(const SizeT position) {
  assert(position <= Size());
  m_file.seekg(static_cast<std::streampos>(position));
}

SizeT
File::TellG() {
  return static_cast<SizeT>(m_file.tellg());
}

SizeT
File::Size() {
  return m_size;
}

void
File::Read(const SizeT nBytes, ANSIChar* bytesOut) {
  assert(TellG() + nBytes <= Size());
  m_file.read(bytesOut, nBytes);
}

void
File::Write(const SizeT nBytes, const ANSIChar* bytesIn) {
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