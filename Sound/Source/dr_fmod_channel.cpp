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

DrChannel*
FMODChannel::get() {
  return reinterpret_cast<DrChannel*>(channel);
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
  result = channel->setVolume(volume);
}

void 
FMODChannel::getVolume(float *volume) {
  result = channel->getVolume(volume);
}

void
FMODChannel::setReverbProperties(Int32 instance,
                                 float wet) {
  result = channel->setReverbProperties(instance,
                                        wet);
}

void
FMODChannel::set3DAttributes(const Vector3D* pos,
                             const Vector3D* vel) {
  result = channel->set3DAttributes(reinterpret_cast<const FMOD_VECTOR*>(pos),
                                    reinterpret_cast<const FMOD_VECTOR*>(vel));
}

}