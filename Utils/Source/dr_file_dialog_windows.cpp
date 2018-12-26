#include "dr_file_dialog.h"

#if DR_PLATFORM == DR_PLATFORM_WINDOWS

#include <algorithm>

#include <Windows.h>
#include <Commdlg.h>

namespace driderSDK {

static OPENFILENAME 
getOpenFileNameStruct(TString& buffer, const TString& title, const TString& filter, const TString& dir)
{
  OPENFILENAME ofn = {};
  
  ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = 0;
	ofn.lpstrDefExt = nullptr;
	ofn.lpstrFile = const_cast<TString::value_type*>(buffer.c_str());
	ofn.nMaxFile = buffer.size();
	ofn.lpstrFilter = filter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = dir.c_str();
	ofn.lpstrTitle = title.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  //Harcoded
  ofn.Flags |= OFN_NOCHANGEDIR;

  auto firstDot = filter.find(_T('.'));

  if (firstDot != TString::npos && firstDot + 3 < filter.size()) {
    ofn.lpstrDefExt = &filter[++firstDot];
  }

  return ofn;
}

TString 
FileDialog::getOpenFileName(const TString& title, const TString& filter, const TString& dir)
{
  TString buffer(256, 0);

  auto ofn = getOpenFileNameStruct(buffer, title, filter, dir);
  
  if (!GetOpenFileName(&ofn)) {
    buffer.clear();
  }

  return buffer;
}

TString
FileDialog::getSaveFileName(const TString& title, const TString& filter, const TString& dir)
{
  TString buffer(256, 0);

  auto ofn = getOpenFileNameStruct(buffer, title, filter, dir);
  
  if (!GetSaveFileName(&ofn)) {
    buffer.clear();
  }

  return buffer;

}

}

#endif