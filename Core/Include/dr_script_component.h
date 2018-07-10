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

  private:
    ScriptEngine *scriptEngine = nullptr;
    asIScriptModule *mod = 0;
    asIScriptObject *obj = 0;
    asITypeInfo *type = 0;    

    std::shared_ptr<ScriptCore> m_script;
};

}