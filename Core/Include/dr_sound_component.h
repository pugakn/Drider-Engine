#pragma once

#include "dr_core_prerequisites.h"

#include <unordered_map>

#include <dr_export_script.h>
#include <dr_id_object.h>

#include "dr_gameComponent.h"

#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class DrSound;
class SoundAPI;
class SoundSystem;
class FMODSoundAPI;
class DrChannelGroup;
class DrChannel;

class Vector3D;

class DR_CORE_EXPORT SoundComponent : public GameComponent, 
                                      public IDClass<SoundComponent>
{

  public:
    SoundComponent(GameObject &gameObject_);

    virtual 
    ~SoundComponent() = default;

    void
    serialize(File &file) override;

    void
    deserialize(TString &data) override;

    void
    onCreate() override;

    void
    onUpdate() override;

    void
    onRender() override;
    
    void
    onDestroy() override;

    virtual UInt32
    getClassID() override;

    GameComponent*
    cloneIn(GameObject& _go) override;


    void
    addSound(TString soundName,
             DrSound* sound);

    void
    setUISounds(Int32 numSounds);

    Int32
    getUISounds();

    void
    removeFromIndexToEnd(UInt32 index);

    TString
    getNameByIndex(UInt32 index);

    void
    play(const TString& soundName);

    static BEGINING_REGISTER(SoundComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)

    result = REGISTER_FOO(SoundComponent,
                          "void play(const TString& in)",
                          asMETHODPR(SoundComponent, play, (const TString&), void));
    
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod("SoundComponent", 
                                                                "GameComponent@ opImplCast()", 
                                                                asFUNCTION((refCast<SoundComponent, GameComponent>)),
                                                                asCALL_CDECL_OBJLAST);

    result = scriptEngine->m_scriptEngine->RegisterObjectMethod("GameComponent",
                                                                "SoundComponent@ opCast()",
                                                                asFUNCTION((refCast<GameComponent, SoundComponent>)),
                                                                asCALL_CDECL_OBJLAST);

    END_REGISTER

  private:
    void
    updateChannel(const Vector3D* pos);
  
  private:
    std::unordered_map<TString, DrSound*> sounds;
    //std::unordered_map<TString, std::unordered_map<TString, DrSound*>> sounds;
    Int32 m_ui_sounds;
  
    FMODSoundAPI* soundAPI;
    SoundSystem* system;
    DrChannelGroup * gChannels;
    DrChannel* channel;
};

}