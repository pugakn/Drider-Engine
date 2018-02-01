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

void
FMODChannel::setPaused(bool paused) {
  channel->setPaused(paused);
}

void
FMODChannel::getPaused(bool *paused) {
  channel->getPaused(paused);
}

void
FMODChannel::setVolume(float volume) {
  channel->setVolume(volume);
}

void 
FMODChannel::getVolume(float *volume) {
  channel->getVolume(volume);
}

}