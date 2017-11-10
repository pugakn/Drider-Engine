#include "dr_codec.h"
#include <dr_parser.h>
#include <dr_loader.h>

namespace driderSDK {
  
void *
Codec::decode(TString pathName) {
  
}

void
Codec::encode(TString pathName,
       ResourceType::E resourceType) {
  return;
}

bool
Codec::isCompatible(TString pathName) {
// No puedo utilizar parser
  TString extension = Parser::strCpyBetween(pathName, _T("."), _T(" "), extension);
  extension = _T("png");
  if(extension == _T("png")) {
    return ResourceType::PNG;
  }
  if (extension == _T("jpg")) {
    return ResourceType::JPG;
  }
  if (extension == _T("fbx")) {
    return ResourceType::FBX_MODEL;
  }
  if (extension == _T("obj")) {
    return ResourceType::OBJ_MODEL;
  }

  return ResourceType::UNSUPPORTED;
}

}