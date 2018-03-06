#pragma once

namespace driderSDK {

#define CONSTRUCT_DESTRUCT_DECL(className)\
  void Construct##className(void *memory);\
  void Destruct##className(void *memory);

#define CONSTRUCT_DESTRUCT_DEF(className)\
  void Construct##className (void *memory) {\
    new(memory) className(); }\
  void Destruct##className (void *memory) {\
    ((className*)memory)->~className(); }

#define COPY_CONSTRUCT_DECL(className)\
  void CopyConstruct##className(const className &other, className *thisPointer);

#define COPY_CONSTRUCT_DEF(className)\
  void CopyConstruct##className(const className &other, className *thisPointer) {\
    new(thisPointer) className(other); }

#define BEGINING_REGISTER(className)\
  Int32 registerFunctions(ScriptEngine* scriptEngine) {\
    Int32 result;\
    result = scriptEngine->m_scriptEngine->RegisterObjectType(#className, sizeof(className), (asOBJ_VALUE | asOBJ_APP_CLASS |\
                                                                                             asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR |\
                                                                                             asOBJ_APP_CLASS_DESTRUCTOR));\
    if(result < 0) {return result;}\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_CONSTRUCT,\
                                                                   "void f()",\
                                                                   asFUNCTION(Construct##className),\
                                                                   asCALL_CDECL_OBJLAST);\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_DESTRUCT,\
                                                                   "void f()",\
                                                                   asFUNCTION(Destruct##className),\
                                                                   asCALL_CDECL_OBJLAST);

#define REGISTER_COPY_CONSTRUCT(className)\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                  asBEHAVE_CONSTRUCT,\
                                                                  "void f(const "#className " & in)",\
                                                                  asFUNCTION(CopyConstruct##className),\
                                                                  asCALL_CDECL_OBJLAST);

#define REGISTER_FOO_0P(className, fooName, rType, rTypeStr)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" " #fooName "()",\
                                                                asMETHOD(className, fooName),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

#define REGISTER_FOO_0P_CONST(className, fooName, rType, rTypeStr)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" " #fooName "()",\
                                                                asMETHODPR(className, fooName, (void) const, rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;


#define REGISTER_FOO_1P(className, fooName, pType, rType, rTypeStr, paramName)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "(" #pType " "#paramName ")",\
                                                                asMETHODPR(className, fooName, (pType), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_1P_CONST(className, fooName, pType, rType, rTypeStr, paramName)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "(" #pType " "#paramName ")",\
                                                                asMETHODPR(className, fooName, (pType) const, rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_2P(className, fooName, pType1, pType2, rType, rTypeStr, paramName1, paramName2)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "("#pType1 " "#paramName1 ","#pType2 " "#paramName2 ")",\
                                                                asMETHODPR(className, fooName, (pType1, pType2), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_2P_CONST(className, fooName, pType1, pType2, rType, rTypeStr, paramName1, paramName2)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "("#pType1 " "#paramName1 ","#pType2 " "#paramName2 ")",\
                                                                asMETHODPR(className, fooName, (pType1, pType2) const, rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;


#define REGISTER_FOO_3P(className, fooName, pType1, pType2, pType3, pType4, rType, rTypeStr, paramName1, paramName2, paramName3)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "("#pType1 " "#paramName1 ", "#pType2 " "#paramName2 ","#pType3 " "#paramName3 ")",\
                                                                asMETHODPR(className, fooName, (pType1, pType2, pType3), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_FOO_4P(className, fooName, pType1, pType2, pType3, pType4, rType, rTypeStr,  paramName1, paramName2, paramName3, paramName4)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#fooName "("#pType1 " "#paramName1 ", "#pType2 " "#paramName2 ","#pType3 " "#paramName3 ", "#pType4 " "#paramName4 ")",\
                                                                asMETHODPR(className, fooName, (pType1, pType2, pType3, pType4), rType),\
                                                                asCALL_THISCALL);\
    if (result < 0) return result;

#define REGISTER_OP(className, sybol, opFoo, pType, rType, rTypeStr)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#opFoo "("#pType ")",\
                                                                asMETHODPR(className, sybol, (pType), rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

#define REGISTER_OP_CONST(className, sybol, opFoo, pType, rType, rTypeStr)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#opFoo "("#pType ")",\
                                                                asMETHODPR(className, sybol, (pType) const, rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

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