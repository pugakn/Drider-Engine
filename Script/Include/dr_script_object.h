#pragma once
#include "dr_script_prerequisites.h"
#include <dr_export_script.h>

#include "dr_script_engine.h"

namespace driderSDK {

class ScriptObject;
ScriptObject *Ref_Factory();

class DR_SCRIPT_EXPORT ScriptObject {
  public:
    Int32 refCount;

    ScriptObject();

    ~ScriptObject();

    void Addref()
    {
      // Increase the reference counter
      refCount++;
    }
    void Release()
    {
      // Decrease ref count and delete if it reaches 0
      if (--refCount == 0)
        delete this;
    }

    void Register(ScriptEngine* scriptEngine) {
      Int32 result;

      result = scriptEngine->m_scriptEngine->RegisterObjectType("ScriptObject", 0, asOBJ_REF);
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("ScriptObject",
                                                                     asBEHAVE_FACTORY,
                                                                     "ScriptObject @f()",
                                                                     asFUNCTION(Ref_Factory),
                                                                     asCALL_CDECL);
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("ScriptObject", 
                                                                     asBEHAVE_ADDREF, 
                                                                     "void f()", 
                                                                     asMETHOD(ScriptObject, Addref),
                                                                     asCALL_THISCALL);
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("ScriptObject", 
                                                                     asBEHAVE_RELEASE, 
                                                                     "void f()", 
                                                                     asMETHOD(ScriptObject, Release),
                                                                     asCALL_THISCALL);

    }

    Int32 property;
};

}