#include "dr_sound_inputs.h"

#include "imgui_stdlib.h"

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
SoundInputs::getInputs() {
  auto& soundComponent = static_cast<SoundComponent&>(m_component);
  if (ImGui::CollapsingHeader(StringUtils::toString(soundComponent.getName()).c_str())) {
    ImGui::Text("Sounds:");
    ImGui::SameLine();
    auto tempSounds = soundComponent.getUISounds();
    if (ImGui::DragInt("##soundsLightComponent", &tempSounds)) {
      soundComponent.setUISounds(tempSounds);
    }

    for(int i = 0; i < tempSounds; i++) {
      TString temp = _T("");
      if (soundComponent.getNameByIndex(i) != L"") {
        temp = soundComponent.getNameByIndex(i);
      }
      ImGui::Text("Sound:"); ImGui::SameLine();
      ImGui::InputText("##soundSoundComponent", &temp, ImGuiInputTextFlags_ReadOnly);
      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_ITEM"))
        {
          char* data = (char*)payload->Data;
          if(FileSystem::GetFileExtension(StringUtils::toTString(data)) == L"mp3") {

            auto system = SoundAPI::instance().API->system;
            auto channel = SoundAPI::instance().API->channel1;
            auto extraInfo = new SoundExtraInfo(reinterpret_cast<SoundSystem*>(system),
                                                reinterpret_cast<DrChannel*>(channel));
            auto ptr = ResourceManager::loadResource(StringUtils::toTString(data), extraInfo);
            auto ptrSound = std::dynamic_pointer_cast<SoundCore>(ptr);

            soundComponent.addSound(ptrSound->getName(), ptrSound->get());
          }
        }
        ImGui::EndDragDropTarget();
      }
    }

  }
}

}