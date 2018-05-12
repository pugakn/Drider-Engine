#include "dr_core_prerequisites.h"

#include <unordered_map>

#include "dr_gameComponent.h"

namespace driderSDK {

class DrSound;
class SoundAPI;
class SoundSystem;
class FMODSoundAPI;
class DrChannelGroup;
class DrChannel;

class Vector3D;

class DR_CORE_EXPORT SoundComponent : public GameComponent {

  public:
    SoundComponent(GameObject &gameObject_);

    virtual 
    ~SoundComponent();
    
    void
    onCreate() override;

    void
    onUpdate() override;

    void
    onRender() override;
    
    void
    onDestroy() override;

    void
    cloneIn(GameObject& _go) override;


    void
    addSound(TString soundName,
             DrSound* sound);

    void
    play(TString soundName);

  private:
    void
    updateChannel(const Vector3D* pos);
  
  private:
    std::unordered_map<TString, DrSound*> sounds;
  
    FMODSoundAPI* soundAPI;
    SoundSystem* system;
    DrChannelGroup * gChannels;
    DrChannel* channel;
};

}