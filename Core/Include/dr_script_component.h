#pragma once
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_keyboard.h>

namespace driderSDK {

class ScriptEnine;
class ScriptCore;

class DR_CORE_EXPORT ScriptComponent : public GameComponent {
  public:
    using ScriptShared = std::shared_ptr<ScriptCore>;
    
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
              TString script,
              TString module);

    void
    start();

    void 
    setScriptLocalProperties();

    void
    onKeyDown(KEY_CODE::E key);

    void
    onKeyUp(KEY_CODE::E key);

  private:
    ScriptEngine *scriptEngine = nullptr;

    ScriptShared script;

    asIScriptModule *mod = 0;
    asIScriptObject *obj = 0;
    asITypeInfo *type = 0;
};

}