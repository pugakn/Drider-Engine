#pragma once
#include <dr_util_prerequisites.h>
#include <dr_gameComponent.h>
#include <dr_script_engine.h>

namespace driderSDK {

class ScriptComponent : public GameComponent {
  public:
    using GameComponent::GameComponent;
    
    virtual
    ~ScriptComponent();

    void
    onCreate() override;

    void
    onUpdate() override;

    /*********
    /* TEMP */
    void
    onRender() override;
    /********/

    void
    onDestroy() override;

    /**
    * Clones the component inside the given gameObject
    */
    void
    cloneIn(GameObject& _go) override;

    void
    addScript(TString name,
              TString script);

    void
    start();

  private:
};

}