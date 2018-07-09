#include "dr_enableObject.h"

namespace driderSDK {

EnableObject::EnableObject() : m_enabled(true) {}

void
EnableObject::setEnabled(bool enabled) {

  m_enabled = enabled;

  if (enabled) {
    onEnabled();
  }
  else {
    onDisabled();
  }
}

bool EnableObject::isEnabled() {
  return m_enabled;
}
}