#include "Spawner.h"
#include <dr_gameObject.h>

namespace driderSDK {

void 
Spawner::setModel(SharedObj model) {
  m_original = model;
}

Spawner::SharedObj 
Spawner::spawn() {
  auto obj = m_original->createInstance();
  *obj = *m_original;
  return  obj;
}

}