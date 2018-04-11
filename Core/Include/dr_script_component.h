#pragma once
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_keyboard.h>

namespace driderSDK {

class ScriptEnine;
class ContextManager;

class DR_CORE_EXPORT ScriptComponent : public GameComponent {
  public:
    
    ScriptComponent(GameObject& _gameObj);
    
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
    compileScript();

  private:
    TString m_scriptName;
    TString m_script;
    TString m_module;

    ContextManager* ctxMag = nullptr;
    ScriptEngine *scriptEngine = nullptr;
};

}