#include "dr_mouse.h"
#include "dr_input_manager.h"

namespace driderSDK {

Mouse::Mouse(Pass) 
  : m_buttonCallbacks(2), 
    m_mouseOIS(nullptr),
    m_helper(*this)
{}

Mouse::~Mouse() {}

Vector2DI 
Mouse::getPosition() {
 
  auto mouse = InputManager::getMouse()->m_mouseOIS;
  
  auto& state = mouse->getMouseState();

  Vector2DI pos;

  pos.x = state.X.abs;
  pos.y = state.Y.abs;

  return pos;
}

Vector2DI Mouse::getDisplacement() {
  
  auto mouse = InputManager::getMouse()->m_mouseOIS;
  
  auto& state = mouse->getMouseState();

  Vector2DI pos;

  pos.x = state.X.rel;
  pos.y = state.Y.rel;

  return pos;
}

bool
Mouse::isButtonDown(MOUSE_BUTTON::E button) {

  auto mouse = InputManager::getMouse()->m_mouseOIS;

  OIS::MouseButtonID buttonOIS = static_cast<OIS::MouseButtonID>(button);

  return mouse->getMouseState().buttonDown(buttonOIS);
}

void 
Mouse::addButtonCallback(MOUSE_INPUT_EVENT::E trigger, 
                         MOUSE_BUTTON::E button, 
                         Callback callback) {
  Mouse* mouse = InputManager::getMouse();

  mouse->m_buttonCallbacks[trigger][button].push_back(callback);
}

void 
Mouse::addMovedCallback(Callback callback) {
  
  Mouse* mouse = InputManager::getMouse();

  mouse->m_movedCallbacks.push_back(callback);
}

void Mouse::callButtonCallbacks(MOUSE_INPUT_EVENT::E trigger, Int32 key) {
  
  auto& callbacks = m_buttonCallbacks[trigger][key];

  for (auto& callback : callbacks) {
    callback();    
  }
}

void Mouse::callMoveCallbacks() {
  
  for(auto& callback : m_movedCallbacks) {
    callback();
  }

}

Mouse::Helper::Helper(Mouse& mouse) : m_parent(mouse) {}

bool 
Mouse::Helper::mouseMoved(const OIS::MouseEvent& arg) {
  
  m_parent.callMoveCallbacks();

  return true;
}

bool 
Mouse::Helper::mousePressed(const OIS::MouseEvent& arg, 
                            OIS::MouseButtonID id) {

  m_parent.callButtonCallbacks(MOUSE_INPUT_EVENT::kButtonPressed, id);

  return true;
}

bool 
Mouse::Helper::mouseReleased(const OIS::MouseEvent & arg, 
                             OIS::MouseButtonID id) {

  m_parent.callButtonCallbacks(MOUSE_INPUT_EVENT::kButtonReleased, id);

  return true;
}

}