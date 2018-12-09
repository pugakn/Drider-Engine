#include "dr_serializable_sound.h"

#include <dr_gameObject.h>

#include <dr_fmod_sound_api.h>
#include <dr_soundSystem.h>
#include <dr_sound_core.h>
#include <dr_channel.h>
#include <dr_channelGroup.h>
#include <dr_soundExtraInfo.h>
#include <dr_sound.h>
#include <dr_sound_component.h>

#include "dr_resource_manager.h"

namespace driderSDK {

void
sSound::load(File &file, std::shared_ptr<GameObject> obj) {
  auto system = SoundAPI::instance().API->system;
  auto channel = SoundAPI::instance().API->channel1;
  auto extraInfo = new SoundExtraInfo(reinterpret_cast<SoundSystem*>(system),
                                      reinterpret_cast<DrChannel*>(channel));

  String name;
  file.m_file >> name;

  Int32 numSounds;
  file.m_file >> numSounds;
  std::vector<String> namesSounds;
  for (int i = 0; i < numSounds; i++) {
    String soundName;
    file.m_file >> soundName;

    ResourceManager::loadResource(StringUtils::toTString(soundName),
                                  extraInfo);
    namesSounds.push_back(soundName);
  }

  auto soundComponent = obj->createComponent<SoundComponent>();
  for (auto sound : namesSounds) {
    auto resource = ResourceManager::instancePtr()->getReferenceT<
                    SoundCore>(StringUtils::toTString(sound));
    soundComponent->addSound(StringUtils::toTString(sound),
                             resource.get()->soundResource);
  }

}

}