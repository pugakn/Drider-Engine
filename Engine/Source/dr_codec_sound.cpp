#include "dr_codec_sound.h"
#include <dr_sound_info.h>
#include <dr_memory.h>

namespace driderSDK {

Codec::UniqueVoidPtr
CodecSound::decode(TString pathName) {
  SoundInfo *info = new SoundInfo();
  info->name = pathName;
  return UniqueVoidPtr(info, &dr_void_deleter<SoundInfo>);
}

bool
CodecSound::encode(TString pathName){
  return false;
}

bool
CodecSound::isCompatible(TString resourceName) {
  if (resourceName == _T("mp3")) {
    return true;
  }
  return false;
}

CompatibleType::E
CodecSound::getType() {
  return CompatibleType::SOUND;
}

}