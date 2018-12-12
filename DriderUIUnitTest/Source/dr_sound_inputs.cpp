#include "dr_sound_inputs.h"
#include <dr_sound_component.h>
#include <dr_resource_manager.h>
#include <dr_fmod_sound_api.h>
#include <dr_soundSystem.h>
#include <dr_sound_core.h>
#include <dr_channel.h>
#include <dr_channelGroup.h>
#include <dr_soundExtraInfo.h>
#include <dr_sound.h>
#include <dr_sound_component.h>
#include <dr_sound_core.h>
namespace driderSDK {

void
SoundInputs::getInputs(TString * response) {

  auto sound = static_cast<SoundComponent&>(m_component);

  (*response) += addInput(_T("0"), _T("number"), _T("Numero de Sonidos"), StringUtils::toTString(sound.getUISounds()));

  for (Int32 i = 0; i < sound.getUISounds(); i++)
  {
    (*response) += addInput(StringUtils::toTString(i+1), _T("droppableFile"), _T("Sonido") + StringUtils::toTString(i), _T("nombresonido[i]"));
  }
  
}

bool
SoundInputs::changeValue(TString &value, TString &id) {
  //cuando hay un cambio de valor verifica si es un sonido y cargalo
    //sound component le manda el sonido
  auto& sound = static_cast<SoundComponent&>(m_component);
  

  if (id != _T("0"))
  {
    //sound.getEctrainfo();

    auto system = SoundAPI::instance().API->system;
    auto channel = SoundAPI::instance().API->channel1;
    auto extraInfo = new SoundExtraInfo(reinterpret_cast<SoundSystem*>(system),
      reinterpret_cast<DrChannel*>(channel));
    auto ptr = ResourceManager::loadResource(value, extraInfo);
    auto ptrSound = std::dynamic_pointer_cast<SoundCore>(ptr);

    sound.addSound( value, ptrSound->get());
  
  }
  else
  {
    sound.setUISounds(StringUtils::toInt(value));
    return true;
  }
  return false;
}

}
