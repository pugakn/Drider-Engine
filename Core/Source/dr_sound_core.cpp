#include "dr_sound_core.h"
#include "dr_sound_info.h"

namespace driderSDK {

void
SoundCore::init(void* pData) {
  SoundInfo *info = static_cast<SoundInfo*>(pData);
  name = info->name;
}

}