#include "dr_file.h"

namespace driderSDK
{

File::File() {
}

File::~File() {
}

bool
File::Open(const TString filename) {
  m_file.open(m_filename, std::ios_base::out | std::ios_base::in);

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
  assert(position >= Size());
  m_file.seekg(position, std::ios_base::beg);
}

SizeT
File::TellG() {
  return m_file.tellg();
}

SizeT
File::Size() {
  std::streampos fsize = 0;
  std::ifstream file(m_filename, std::ios::binary);
  fsize = file.tellg();
  file.seekg(0, std::ios::end);
  fsize = file.tellg() - fsize;
  file.close();

  return fsize;
}

void
File::Read(const SizeT nBytes, ANSIChar* bytesOut) {
  assert(TellG() + nBytes > Size());
  m_file.read(bytesOut, nBytes);
}

void
File::Write(const SizeT nBytes, ANSIChar * bytesIn) {
  m_file.write(bytesIn, nBytes);
}

TString
File::GetAsString(const SizeT nBytes) {
  assert(TellG() + nBytes > Size());

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