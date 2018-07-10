#include "dr_id_object.h"

driderSDK::UInt32
IDBase::getNextID() {
  static driderSDK::UInt32 nextID = 0;
  return nextID++;
}

namespace driderSDK {


IDObject::IDObject() : m_id(IDDispatcher<UInt32>::get()) 
{}

}