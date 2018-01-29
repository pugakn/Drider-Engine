#include "dr_input_manager.h"
#include "dr_input_mouse.h"
#include "dr_input_keyboard.h"
#include "dr_input_joystick.h"
namespace driderSDK {
  void InputManager::init(std::size_t winHandle)
  {
    m_mngr = OIS::InputManager::createInputSystem(winHandle);
  }
  void InputManager::destroy()
  {
    for (int i = 0; i < m_objects.size(); ++i) {
      destroyInputObject(i);
    }
    OIS::InputManager::destroyInputSystem(m_mngr);
  }

  UInt32 InputManager::createInputObject(InputObjectType::E type)
  {
    OIS::Type internalType;
    InputObject* inputObject = nullptr;
    switch (type)
    {
    case driderSDK::InputObjectType::kKeyboard:
      internalType = OIS::Type::OISKeyboard;
      OIS::Object* obj = m_mngr->createInputObject(internalType, true);
      inputObject = new KeyboardInput();
      inputObject->internalInit(obj);
      break;
    case driderSDK::InputObjectType::kMouse:
      OIS::Object* obj = m_mngr->createInputObject(internalType, true);
      inputObject = new MouseInput();
      inputObject->internalInit(obj);
      break;
    case driderSDK::InputObjectType::kJoystick:
      OIS::Object* obj = m_mngr->createInputObject(internalType, true);
      //inputObject = new JoystickInput();
      inputObject->internalInit(obj);
      break;
    default:
      break;
    }
    inputObject->m_type = type;
    m_objects.push_back(inputObject);
    return m_objects.size() - 1;
  }

  void InputManager::destroyInputObject(UInt32 objID)
  {
    if (m_objects[objID] == nullptr)
      return;
    m_mngr->destroyInputObject(m_objects[objID]->m_obj);
    delete m_objects[objID];
    m_objects[objID] = nullptr;
  }

  int InputManager::getNumberOfDevices(InputObjectType::E type)
  {
    OIS::Type internalType;
    switch (type)
    {
    case driderSDK::InputObjectType::kKeyboard:
      internalType = OIS::Type::OISKeyboard;
      break;
    case driderSDK::InputObjectType::kMouse:
      break;
    case driderSDK::InputObjectType::kJoystick:
      break;
    default:
      break;
      return m_mngr->getNumberOfDevices(internalType);
  }

}

  InputObject * driderSDK::InputManager::getInputObjectByID(UInt32 id)
  {
    return m_objects[id];
  }
