#include "dr_sound_component.h"

#include <dr_sound.h>
#include <dr_fmod_sound_api.h>
#include <dr_fmod_channel.h>
#include <dr_fmod_channelGroup.h>
#include <dr_soundSystem.h>
#include <dr_vector3d.h>

#include "dr_gameObject.h"

namespace driderSDK {

SoundComponent::SoundComponent(GameObject &gameObject_) 
  : GameComponent(gameObject_, _T("SoundComponent")),
    m_ui_sounds(0)
{}

void
SoundComponent::serialize(File &file) {
  file.m_file << SerializableTypeID::Sound << "\n";
  file.m_file << StringUtils::toString(getName()) << "\n";

  file.m_file << sounds.size() << "\n";
  for(auto sound: sounds) {
    file.m_file << StringUtils::toString(sound.first) << "\n";
  }  

}

void
SoundComponent::deserialize(TString &data) {

}

void
SoundComponent::onCreate() {
  soundAPI = SoundAPI::instance().API;

  system = soundAPI->system;
  gChannels = soundAPI->masterGroup->get();
  channel = soundAPI->channel1->get();
}

void
SoundComponent::onUpdate() {
  /*Transform trans = getGameObject().getTransform();

  const Vector3D vel(0.0f, 0.0f, 0.0f);*/

  //updateChannel(const_cast<Vector3D*>(&trans.getPosition()));
                
  
}

void
SoundComponent::onRender() {

}


void
SoundComponent::onDestroy(){

}

UInt32 SoundComponent::getClassID()
{
  return CLASS_NAME_ID(SoundComponent);
}

GameComponent*
SoundComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<SoundComponent>();
}

void
SoundComponent::addSound(TString soundName,
                         DrSound* sound) {
  sounds.insert({soundName,
                sound});
}

void
SoundComponent::setUISounds(Int32 numSounds) {
  m_ui_sounds = numSounds;
}

Int32
SoundComponent::getUISounds() {
  return m_ui_sounds;
}

TString
SoundComponent::getNameByIndex(UInt32 index) {
  UInt32 count = 0;
  TString name = L"";

  for (auto sound : sounds) {
    if (count == index) {
      name = sound.first;
    }
    count++;
  }

  return name;
}

void
SoundComponent::removeFromIndexToEnd(UInt32 index) {
   sounds.erase(sounds.find(getNameByIndex(index)),
                sounds.end());
}

void
SoundComponent::play(const TString& soundName) {
  
  std::unordered_map<TString, 
                     DrSound*>::iterator 
                     it = sounds.find(soundName);


  
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

void
SoundComponent::updateChannel(const Vector3D* pos) {
  
  const Vector3D p(1.0f, 0.0f, 0.0f);
  const Vector3D v(0.0f,0.0f,0.0f);
  
  channel->set3DAttributes(&p,
                           &v);
}

}