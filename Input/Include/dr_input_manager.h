#pragma once
#include "dr_input_prerequisites.h"
#include <OIS/OISInputManager.h>
#include "dr_input_object.h"
namespace driderSDK {
class DR_INPUT_EXPORT InputManager {
  void init(std::size_t winHandle);
  void destroy();

  InputObject* createInputObject(InputObjectType::E iType);
  void destroyInputObject(InputObject* obj);

  int getNumberOfDevices(InputObjectType::E iType);
  InputObjectType::E listFreeDevices();

private:
  OIS::InputManager* m_mngr;
};
}