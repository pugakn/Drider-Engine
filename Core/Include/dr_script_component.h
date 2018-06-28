#pragma once
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_keyboard.h>

namespace driderSDK {

class ScriptEnine;
class ScriptCore;

class DR_CORE_EXPORT ScriptComponent : public GameComponent {
  public:
    
    //ScriptComponent(GameObject& _gameObj);
    
    ScriptComponent(GameObject &gameObject,
                    std::shared_ptr<ScriptCore> _script);
    
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

    virtual UInt32
    getClassID() override;

    /**
    * Clones the component inside the given gameObject
    */
    void
    cloneIn(GameObject& _go) override;

    void
    initScript();

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
    asIScriptModule *mod = 0;
    asIScriptObject *obj = 0;
    asITypeInfo *type = 0;    

    std::shared_ptr<ScriptCore> m_script;
};

}