#include "dr_id_object.h"

namespace driderSDK {

IDObject::IDObject() : m_id(IDDispatcher<UInt32>::get()) 
{}

}