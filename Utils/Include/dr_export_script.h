#pragma once

namespace driderSDK {

#define GETSET(varName, pType)\
pType m_##varName;\
pType get_##varName() {\
  return m_##varName;\
}
//void set_##varName(pType param1) {\
//  m_##varName = param1;\
//}\

//r = engine->RegisterObjectMethod("object", "int getAttr(int) const", asMETHODPR(Object, getAttr, (int) const, int), asCALL_THISCALL);
}