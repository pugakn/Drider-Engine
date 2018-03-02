#pragma once

namespace driderSDK {

#define GETSET(varName, pType)\
  pType m_##varName;\
  pType get_##varName() {\
    return m_##varName; }\
  void set_##varName(pType param1) {\
    m_##varName = param1; } 

#define REGISTER_BEGIN(className, ptrEngineScript)\
  Int32 register#className() {\
    Int32 r;
    r = ptrEngineScript->RegisterObjectType(#className, 0, asOBJ_REF);\

#define REGISTER_FOO1P(fooName, pType, rType) \
    r = ptrEngineScript->RegisterObjectMethod(#className,\
                                              #rType" "#fooName(#pType" param1)",\
                                              asMETHODPR(className, fooName, (pType), rType),\
                                              asCALL_THISCALL);

#define REGISTER_FOO2P(fooName, pType1, pType2, rType) \
    r = ptrEngineScript->RegisterObjectMethod(#className,\
                                              #rType" "#fooName(#pType1" param1, "#pType2" param2)",\
                                              asMETHODPR(className, fooName, (pType1, pType2), rType),\
                                              asCALL_THISCALL);

#define REGISTER_END \
    return r; }

//r = engine->RegisterObjectMethod("object", "int getAttr(int) const", asMETHODPR(Object, getAttr, (int) const, int), asCALL_THISCALL);
}