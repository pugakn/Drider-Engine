#include "..\Include\dr_name_object.h"

namespace driderSDK {

NameObject::NameObject(const TString & _name) : m_name(_name) {
}

void
NameObject::setName(const TString& name) {
  m_name = name;
}

const TString& 
NameObject::getName() const {
  return m_name;
}

}