#include "dr_codec_material.h"
#include <dr_material_info.h>
#include <dr_memory.h>

namespace driderSDK {

Codec::UniqueVoidPtr 
CodecMaterial::decode(TString pathName)
{
  MaterialInfo *info = new MaterialInfo();
  info->name = pathName;

  

  return UniqueVoidPtr(info, &dr_void_deleter<MaterialInfo>);
}

bool
CodecMaterial::encode(TString pathName) {
  return false;
}

bool
CodecMaterial::isCompatible(TString resourceName) {
  if (resourceName == _T("mat")) {
    return true;
  }
  return false;
}

CompatibleType::E
CodecMaterial::getType() {
  return CompatibleType::MATERIAL;
}
}