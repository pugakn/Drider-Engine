#include "dr_fmod_channel.h"

namespace driderSDK {
 
void*
FMODChannel::getReference() {
  return channel;
}

void**
FMODChannel::getObjectReference() {
  return reinterpret_cast<void**>(&channel);
}

}