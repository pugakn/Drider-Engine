#include "dr_input_manager.h"
#include <OIS/OISInputManager.h>
#include "dr_input_mouse.h"
#include "dr_input_keyboard.h"
#include "dr_input_joystick.h"

namespace driderSDK {

void
InputManager::init(std::size_t winHandle, bool nonExlusive) {
  if (nonExlusive) {
    OIS::ParamList pl;
    pl.insert(std::make_pair(std::string("WINDOW"), std::to_string(winHandle)));
#if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
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
  for (size_t i = 0; i < m_objects.size(); ++i) {
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

  inputObject->m_type = type;
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

  m_mngr->destroyInputObject(m_objects[objID]->m_obj);
  delete m_objects[objID];

  m_objects[objID] = nullptr;
}

int
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

void InputManager::registerAllActiveDevices()
{
  int num = getNumberOfDevices(InputObjectType::kKeyboard);
  for (int i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kKeyboard);
  }
  num = getNumberOfDevices(InputObjectType::kMouse);
  for (int i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kMouse);
  }
  num = getNumberOfDevices(InputObjectType::kJoystick);
  for (int i = 0; i < num; ++i)
  {
    createInputObject(InputObjectType::kJoystick);
  }
}

MouseInput * InputManager::getMouse()
{
  return m_mouseDevices[0];
}

KeyboardInput * InputManager::getKeyboard()
{
  return m_keyboardDevices[0];
}

UInt32 InputManager::getNumOfJoysticks()
{
  return m_joystickDevices.size();
}

JoystickInput * InputManager::getJoystick(UInt32 id)
{
  return m_joystickDevices[id];
}

InputObject* driderSDK::InputManager::getInputObjectByID(UInt32 id) {
  if (id >= m_objects.size()) {
    return nullptr;
  }

  return m_objects[id];
}

}