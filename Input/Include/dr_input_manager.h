#pragma once
#include "dr_input_prerequisites.h"
#include <OIS/OISInputManager.h>
#include <vector>
#include "dr_input_object.h"
namespace driderSDK {
class DR_INPUT_EXPORT InputManager {
  void init(std::size_t winHandle);
  void destroy();

  UInt32 createInputObject(InputObjectType::E type);
  void destroyInputObject(UInt32 objID);

  int getNumberOfDevices(InputObjectType::E type);
  //std::vrector<InputObjectType::E> listFreeDevices();

  InputObject* getInputObjectByID(UInt32 id);

private:
  std::vector<InputObject*> m_objects;
  OIS::InputManager* m_mngr;
};
}