#pragma once

namespace driderSDK {

template<class A, class B>
B* refCast(A* a)
{
  // If the handle already is a null handle, then just return the null handle
  if (!a) return 0;
  // Now try to dynamically cast the pointer to the wanted type
  B* b = dynamic_cast<B*>(a);
  return b;
}

#define VALUE_FLAGS (asOBJ_VALUE | asOBJ_APP_CLASS |\
                     asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR |\
                     asOBJ_APP_CLASS_DESTRUCTOR)

#define CONSTRUCT_DESTRUCT_DECL(className)\
  void Construct##className(void *memory);\
  void Destruct##className(void *memory);

#define CONSTRUCT_DESTRUCT_DEF(className)\
  void Construct##className (void *memory) {\
    new(memory) className(); }\
  void Destruct##className (void *memory) {\
    ((className*)memory)->~className(); }

//#define REF_CONSTRUCT(className)\
//  class className;\
//  className* Ref_##className() {\
//    return new className();/
//  }

#define COPY_CONSTRUCT_DECL(className)\
  void CopyConstruct##className(const className &other, className *thisPointer);

#define COPY_CONSTRUCT_DEF(className)\
  void CopyConstruct##className(const className &other, className *thisPointer) {\
    new(thisPointer) className(other); }

#define CONSTRUCT_3P_DECL(className, pType1, pType2, pType3)\
  void ConstructFromTwoFloats##className(pType1 a, pType2 b, pType3 c, className *thisPointer);

#define CONSTRUCT_3P_DEF(className, pType1, pType2, pType3)\
  void ConstructFromTwoFloats##className(pType1 a, pType2 b, pType3 c, className *thisPointer) {\
  new(thisPointer) className(a, b, c); }

#define BEGINING_DEFAULT_REGISTER\
  Int32 registerFunctions(ScriptEngine* scriptEngine) {\
    Int32 result;

#define BEGINING_REGISTER(className, size, flags)\
  Int32 registerFunctions(ScriptEngine* scriptEngine) {\
    Int32 result;\
    result = scriptEngine->m_scriptEngine->RegisterObjectType(#className, size, flags);\
    if(result < 0) {return result;}

#define REGISTER_CONSTRUC_DESTRUCT(className)\
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

#define REGISTER_CONSTRUCT_3P(className, pType1, pType2, pType3)\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                  asBEHAVE_CONSTRUCT,\
                                                                  "void f("#pType1 ", "#pType2 ", "#pType3 ")",\
                                                                  asFUNCTION(ConstructFromTwoFloats##className),\
                                                                  asCALL_CDECL_OBJLAST);

#define REGISTER_REF(className) \
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                     asBEHAVE_FACTORY,\
                                                                     #className" @f()",\
                                                                     asFUNCTION(Ref_##className),\
                                                                     asCALL_CDECL);\
      if(result < 0) return result;\
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                     asBEHAVE_ADDREF,\
                                                                     "void f()",\
                                                                     asMETHOD(className, addRef),\
                                                                     asCALL_THISCALL);\
      if(result < 0) return result;\
      result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                     asBEHAVE_RELEASE,\
                                                                     "void f()",\
                                                                     asMETHOD(className, release),\
                                                                     asCALL_THISCALL);\
      if (result < 0) return result;

#define REGISTER_REF_NOCOUNT(className)\
    result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour(#className,\
                                                                   asBEHAVE_FACTORY,\
                                                                   #className " @f()",\
                                                                   asFUNCTION(Ref_##className),\
                                                                   asCALL_CDECL);

// METHODS FUNCTIONS
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

#define REGISTER_FOO(className, strFooDef, method)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                strFooDef,\
                                                                method,\
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

#define REGISTER_OP(className, sybol, opFoo, pType, rType, rTypeStr, paramName)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#opFoo "("#pType " "#paramName ")",\
                                                                asMETHODPR(className, sybol, (pType), rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

#define REGISTER_OP_CONST(className, sybol, opFoo, pType, rType, rTypeStr, paramName)\
    result = scriptEngine->m_scriptEngine->RegisterObjectMethod(#className,\
                                                                rTypeStr" "#opFoo "("#pType " "#paramName ")",\
                                                                asMETHODPR(className, sybol, (pType) const, rType),\
                                                                asCALL_THISCALL);\
    if(result < 0) return result;

// ENUMS
#define REGISTER_ENUM(nameEnum)\
    result = scriptEngine->m_scriptEngine->RegisterEnum(#nameEnum);
#define REGISTER_ENUM_VALUE(nameEnum, enumC, valueName)\
    result = scriptEngine->m_scriptEngine->RegisterEnumValue(#nameEnum,\
                                                             #valueName,\
                                                             (Int32)enumC::valueName);

//GLOBAL FUNCTIONS

#define REGISTER_GLO_FOO_0P(foo, fooName, rType, strRType)\
    result = scriptEngine->m_scriptEngine->RegisterGlobalFunction (strRType " "#fooName "()",\
                                                                   asFUNCTIONPR(foo, (void), rType),\
                                                                   asCALL_CDECL);
#define REGISTER_GLO_FOO_1P(foo, fooName, pType, asPType, rType, strRType)\
    result = scriptEngine->m_scriptEngine->RegisterGlobalFunction (strRType " "#fooName "("#asPType ")",\
                                                                   asFUNCTIONPR(foo, (pType), rType),\
                                                                   asCALL_CDECL);

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