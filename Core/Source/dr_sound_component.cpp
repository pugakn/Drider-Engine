#include "dr_sound_component.h"

#include <dr_sound.h>
#include <dr_fmod_sound_api.h>
#include <dr_fmod_channel.h>
#include <dr_fmod_channelGroup.h>
#include <dr_soundSystem.h>

namespace driderSDK {

SoundComponent::SoundComponent(GameObject &gameObject_) 
  : GameComponent(gameObject_, _T("SoundComponent")) 
{}

SoundComponent::~SoundComponent() 
{}

void
SoundComponent::onCreate() {

}

void
SoundComponent::onUpdate() {

}

void
SoundComponent::onRender() {

}


void
SoundComponent::onDestroy(){

}

void
SoundComponent::cloneIn(GameObject& _go) {

}

void
SoundComponent::addSound(TString soundName,
                         DrSound* sound) {
  sounds.insert({soundName,
                sound});
}

void
SoundComponent::play(TString soundName) {
  
  std::unordered_map<TString, 
                     DrSound*>::iterator 
                     it = sounds.find(soundName);

  auto soundAPI = SoundAPI::instance().API;

  auto system = soundAPI->system;
  auto gChannels = soundAPI->masterGroup->get();
  auto channel = soundAPI->channel1->get();
  
  if(it != sounds.end()) {
    soundAPI->system->playSound(it->second->get(),
                                gChannels,
                                false,
                                &channel);                                              
  } else {
    TString err = _T("Sound ") + soundName + _T(" no exists");
    Logger::addLog(err);
  }

  
}

}