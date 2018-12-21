#pragma once

#include <dr_id_object.h>
#include <dr_keyboard.h>

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include "dr_gameComponent.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class ScriptEnine;
class ScriptCore;

class DR_CORE_EXPORT ScriptComponent : public GameComponent,
                                       public IDClass<ScriptComponent> {
  public:
    
    //ScriptComponent(GameObject& _gameObj);
    
    ScriptComponent(GameObject &gameObject,
                    std::shared_ptr<ScriptCore> _script);

    ScriptComponent(GameObject &gameObject);
    
    virtual
    ~ScriptComponent();

    void
    serialize(File &file) override;

    void
    deserialize(TString &data) override;

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
    GameComponent*
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

    asIScriptObject*
    getScript();

    FORCEINLINE bool
    isEmpty() {
      return (m_script == NULL) || (type == NULL);
    };
    void
    setScript(std::shared_ptr<ScriptCore> _script);

    void
    addScriptSection();

    FORCEINLINE ScriptEngine*
    getScriptEngine() {
      return scriptEngine;
    };

    FORCEINLINE void
    discard() {
      obj = 0;
      type = 0;
    }

    static BEGINING_REGISTER(ScriptComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)

    result = scriptEngine->m_scriptEngine->RegisterInterface("IScript");
    result = REGISTER_FOO(ScriptComponent,
                          "IScript@ getScript()",
                          asMETHOD(ScriptComponent, getScript))

    result = scriptEngine->m_scriptEngine->RegisterObjectMethod("ScriptComponent",
                                                                "GameComponent@ opImplCast()",
                                                                asFUNCTION((refCast<ScriptComponent, GameComponent>)),
                                                                asCALL_CDECL_OBJLAST);

    result = scriptEngine->m_scriptEngine->RegisterObjectMethod("GameComponent",
                                                                "ScriptComponent@ opCast()",
                                                                asFUNCTION((refCast<GameComponent, ScriptComponent>)),
                                                                asCALL_CDECL_OBJLAST);
    
    END_REGISTER
    std::shared_ptr<ScriptCore> m_script;

  private:
    ScriptEngine *scriptEngine = nullptr;
    asIScriptModule *mod = 0;
    asIScriptObject *obj = 0;
    asITypeInfo *type = 0;    

};

}