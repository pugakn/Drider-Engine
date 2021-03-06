#include "dr_input_manager.h"
#include <OIS/OISInputManager.h>
#include "dr_input_mouse.h"
#include "dr_input_keyboard.h"
#include "dr_input_joystick.h"

namespace driderSDK {

void
InputManager::init(SizeT winHandle, bool nonExlusive) {
  if (nonExlusive) {
    OIS::ParamList pl;
    pl.insert(std::make_pair(String("WINDOW"), std::to_string(winHandle)));
#if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(String("w32_mouse"), String("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(String("w32_mouse"), String("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(String("w32_keyboard"), String("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(String("w32_keyboard"), String("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(String("x11_mouse_grab"), String("false")));
    pl.insert(std::make_pair(String("x11_mouse_hide"), String("false")));
    pl.insert(std::make_pair(String("x11_keyboard_grab"), String("false")));
    pl.insert(std::make_pair(String("XAutoRepeatOn"), String("true")));
#endif
    m_mngr = OIS::InputManager::createInputSystem(pl);
  }
  else {
    m_mngr = OIS::InputManager::createInputSystem(winHandle);
  }
  registerAllActiveDevices();
}

void
InputManager::destroy() {
  for (SizeT i = 0; i < m_objects.size(); ++i) {
    destroyInputObject(i);
  }

  OIS::InputManager::destroyInputSystem(m_mngr);
}

void 
InputManager::captureAll()
{
  for (auto &it : m_objects) {
    if (it!= nullptr)
      it->capture();
  }
}

UInt32
InputManager::createInputObject(InputObjectType::E type) {
  OIS::Type internalType;
  OIS::Object* obj;
  InputObject* inputObject = nullptr;

  switch (type)
  {
  case driderSDK::InputObjectType::kKeyboard:
    internalType = OIS::Type::OISKeyboard;
    obj = m_mngr->createInputObject(internalType, true);
    inputObject = new KeyboardInput();
    inputObject->internalInit(obj);
    m_keyboardDevices.push_back((KeyboardInput*)inputObject);
    break;
  case driderSDK::InputObjectType::kMouse:
    internalType = OIS::Type::OISMouse;
    obj = m_mngr->createInputObject(internalType, true);
    inputObject = new MouseInput();
    inputObject->internalInit(obj);
    m_mouseDevices.push_back((MouseInput*)inputObject);
    break;
  case driderSDK::InputObjectType::kJoystick:
    internalType = OIS::Type::OISJoyStick;
    obj = m_mngr->createInputObject(internalType, true);
    inputObject = new JoystickInput();
    inputObject->internalInit(obj);
    m_joystickDevices.push_back((JoystickInput*)inputObject);
    break;
  default:
    break;
  }

  m_objects.push_back(inputObject);

  return m_objects.size() - 1;
}

void
InputManager::destroyInputObject(UInt32 objID) {
  if (objID >= m_objects.size()) {
    return;
  }
  if (m_objects[objID] == nullptr) {
    return;
  }

  m_mngr->destroyInputObject(m_objects[objID]->obj);
  delete m_objects[objID];

  m_objects[objID] = nullptr;
}

Int32
InputManager::getNumberOfDevices(InputObjectType::E type) {
  OIS::Type internalType;

  switch (type)
  {
  case driderSDK::InputObjectType::kKeyboard:
    internalType = OIS::Type::OISKeyboard;
    break;
  case driderSDK::InputObjectType::kMouse:
    internalType = OIS::Type::OISMouse;
    break;
  case driderSDK::InputObjectType::kJoystick:
    internalType = OIS::Type::OISJoyStick;
    break;
  default:
    internalType = OIS::Type::OISUnknown;
    break;
  }

  return m_mngr->getNumberOfDevices(internalType);
}

void 
InputManager::registerAllActiveDevices() {
  Int32 num = getNumberOfDevices(InputObjectType::kKeyboard);
  for (Int32 i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kKeyboard);
  }
  num = getNumberOfDevices(InputObjectType::kMouse);
  for (Int32 i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kMouse);
  }
  num = getNumberOfDevices(InputObjectType::kJoystick);
  for (Int32 i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kJoystick);
  }
}

MouseInput* 
InputManager::getMouse() {
  return m_mouseDevices[0];
}

KeyboardInput* 
InputManager::getKeyboard() {
  return m_keyboardDevices[0];
}

UInt32 
InputManager::getNumOfJoysticks() {
  return m_joystickDevices.size();
}

JoystickInput* 
InputManager::getJoystick(UInt32 id) {
  return m_joystickDevices[id];
}

InputObject* 
InputManager::getInputObjectByID(UInt32 id) {
  if (id >= m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

}