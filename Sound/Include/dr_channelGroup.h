#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {
  class DR_SOUND_EXPORT DrChannelGroup {
    public:
      DrChannelGroup() {}
      virtual ~DrChannelGroup() {}
     
      virtual void*
      getReference() = 0;
  };
}