#include "dr_codec.h"
#include <dr_parser.h>

namespace driderSDK {
  
std::shared_ptr<Resource>
Codec::decode(TString pathName) {
  return std::shared_ptr<Resource>();
}

void
Codec::encode(TString pathName,
       ResourceType::E resourceType) {
  return;
}

ResourceType::E
Codec::isCompatible(TString pathName) {
// No puedo utilizar parser
  TString extension;// = Parser::strCpyBetween(pathName, _T("."), _T(" "), extension);
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