#include "dr_mouse.h"
#include "dr_input_manager.h"

namespace driderSDK {

Mouse::Mouse(Pass) : m_callbacks(3), m_state(nullptr) {}

Mouse::~Mouse() {}

Vector2DI 
Mouse::getPosition(bool absolute) {
 
  Mouse* mouse = InputManager::getMouse();
  
  Vector2DI pos;

  if (absolute) {
    pos.x = mouse->m_state->X.abs;
    pos.y = mouse->m_state->Y.abs;
  }
  else {
    pos.x = mouse->m_state->X.rel;
    pos.y = mouse->m_state->Y.rel;
  }

  return pos;
}

bool
Mouse::isButtonDown(MOUSE_BUTTON::E button) {

  Mouse* mouse = InputManager::getMouse();

  OIS::MouseButtonID buttonOIS = static_cast<OIS::MouseButtonID>(button);

  return mouse->m_state->buttonDown(buttonOIS);
}

void
Mouse::addCallback(MOUSE_EVENT::E trigger, 
                   MOUSE_BUTTON::E button, 
                   Callback callback) {

  Mouse* mouse = InputManager::getMouse();

  if (trigger == MOUSE_EVENT::kMouseMoved) {
    const Int32 key = 18;
    mouse->m_callbacks[trigger][key].push_back(callback);
  }
  else {
    mouse->m_callbacks[trigger][button].push_back(callback);
  }

}

bool 
Mouse::mouseMoved(const OIS::MouseEvent& arg) {

  const Int32 key = 18;

  callCallbacks(MOUSE_EVENT::kMouseMoved, key);

  return true;
}

bool 
Mouse::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

  callCallbacks(MOUSE_EVENT::kButtonPressed, id);

  return true;
}

bool 
Mouse::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

  callCallbacks(MOUSE_EVENT::kButtonReleased, id);

  return true;
}

void 
Mouse::callCallbacks(MOUSE_EVENT::E trigger, Int32 key) {

  /*EventArgs args;

  args.pressedButtons = m_state->buttons;
  args.absolutePos.x = m_state->X.abs;
  args.absolutePos.y = m_state->Y.abs;
  args.relativePos.x = m_state->X.rel;
  args.relativePos.y = m_state->Y.rel;*/

  auto& callbacks = m_callbacks[trigger][key];
  
  for(auto& callback : callbacks) {
    callback();
  }

}




}