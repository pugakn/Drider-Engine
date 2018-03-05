#pragma once

namespace driderSDK {

#define DETECT_TYPE(type) (#type == "Int32") ? "int" : #type
#define REF(r) (#r == "ref") ? "ref" : " "

#define CREATE_REF_FACTORY_DECL(className)\
  class className;\
  className *Ref_Factory_##className();

#define CREATE_REF_FACTORY_FUNC(className)\
  className *Ref_Factory_##className() {\
    return new className();\
  }

#define CREATE_REF_FACTORY_DECL_3P(className, pType1, pType2, pType3)\
  className *Ref_Factory_Param_##className(pType1 param1, pType2 param2, pType3 param3);

#define CREATE_REF_FACTORY_FUNC_3P(className, pType1, pType2, pType3)\
  className *Ref_Factory_Param_##className(pType1 param1, pType2 param2, pType3 param3) {\
    return new className(param1, param2, param3);\
  }

#define GETSET(varName, pType)\
  pType m_##varName;\
  pType get_##varName() {\
    return m_##varName; }\
  void set_##varName(pType param1) {\
    m_##varName = param1; } 

#define DEFAULT_CONSTRUCTOR(className)\
  private:\
    Int32 refCount;\
  public:\
    className() : refCount(1) {}\
    void AddRef() {\
      refCount++;\
    }\
    void Release() {\
      if (--refCount == 0)\
        delete this;\
    }

#define BEGINING_REGISTER(className)\
  Int32 registerFunctions(ScriptEngine* scriptEngine) {\
    Int32 result;\
    result = scriptEngine->m_scriptEngine->RegisterObjectType(#className, sizeof(className), asOBJ_REF);\
    if(result < 0) {return result;}\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_FACTORY,\
                                                                   #className"@ f()",\
                                                                   asFUNCTION(Ref_Factory_##className),\
                                                                   asCALL_CDECL);\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_ADDREF,\
                                                                   "void f()",\
                                                                   asMETHOD(className, AddRef),\
                                                                   asCALL_THISCALL);\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_RELEASE,\
                                                                   "void f()",\
                                                                   asMETHOD(className, Release),\
                                                                   asCALL_THISCALL);

#define REGISTER_FACTORY_3P(className, pType1, pType2, pType3)\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_FACTORY,\
                                                                   #className"@ f("#pType1" param1,"#pType2" param2,"#pType3" param3)",\
                                                                   asFUNCTIONPR(Ref_Factory_Param_##className, (pType1, pType2, pType3), className*),\
                                                                   asCALL_CDECL);

#define REGISTER_FOO_0P(className, fooName, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" " #fooName "()",\
                                                                asMETHODPR(className, fooName, (void), rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

#define REGISTER_FOO_0P_CONST(className, fooName, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" " #fooName "()",\
                                                                asMETHODPR(className, fooName, (void) const, rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;


#define REGISTER_FOO_1P(className, fooName, pType, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" "#fooName"(" #pType " param1)",\
                                                                asMETHODPR(className, fooName, (pType), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_1P_CONST(className, fooName, pType, rType, r)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)r#fooName"(" #pType " param1)",\
                                                                asMETHODPR(className, fooName, (pType) const, rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_2P(className, fooName, pType1, pType2, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" "#fooName"("#pType1 " param1,"#pType2 " param2)",\
                                                                asMETHODPR(className, fooName, (pType1, pType2), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;


#define REGISTER_FOO_3P(className, fooName, pType1, pType2, pType3, pType4, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" "#fooName"("#pType1 " param1, "#pType2 " param2,"#pType3 " param3)",\
                                                                asMETHODPR(className, fooName, (pType1, pType2, pType3), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_4P(className, fooName, pType1, pType2, pType3, pType4, rType)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                DETECT_TYPE(rType)" "#fooName"("#pType1 " param1, "#pType2 " param2,"#pType3 " param3, "#pType4 " param4)",\
                                                                asMETHODPR(className, fooName, (pType1, pType2, pType3, pType4), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define END_REGISTER\
    return result;\
  }

/*
asSUCCESS                              =  0,
asERROR                                = -1,
asCONTEXT_ACTIVE                       = -2,
asCONTEXT_NOT_FINISHED                 = -3,
asCONTEXT_NOT_PREPARED                 = -4,
asINVALID_ARG                          = -5,
asNO_FUNCTION                          = -6,
asNOT_SUPPORTED                        = -7,
asINVALID_NAME                         = -8,
asNAME_TAKEN                           = -9,
asINVALID_DECLARATION                  = -10,
asINVALID_OBJECT                       = -11,
asINVALID_TYPE                         = -12,
asALREADY_REGISTERED                   = -13,
asMULTIPLE_FUNCTIONS                   = -14,
asNO_MODULE                            = -15,
asNO_GLOBAL_VAR                        = -16,
asINVALID_CONFIGURATION                = -17,
asINVALID_INTERFACE                    = -18,
asCANT_BIND_ALL_FUNCTIONS              = -19,
asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20,
asWRONG_CONFIG_GROUP                   = -21,
asCONFIG_GROUP_IS_IN_USE               = -22,
asILLEGAL_BEHAVIOUR_FOR_TYPE           = -23,
asWRONG_CALLING_CONV                   = -24,
asBUILD_IN_PROGRESS                    = -25,
asINIT_GLOBAL_VARS_FAILED              = -26,
asOUT_OF_MEMORY                        = -27,
asMODULE_IS_IN_USE                     = -28
*/

//r = engine->RegisterObjectMethod("object", "int getAttr(int) const", asMETHODPR(Object, getAttr, (int) const, int), asCALL_THISCALL);
}

/*    result = scriptEngine->m_scriptEngine->RegisterObjectMethod("Camera",
                                                                "float get_variable()",
                                                                asMETHODPR(Camera, get_variable, (void), float),
                                                                asCALL_THISCALL);*/