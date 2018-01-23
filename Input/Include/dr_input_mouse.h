#pragma once
#include "dr_input_prerequisites.h"
#include <OIS\OISMouse.h>
#include <dr_vector2di.h>
#include <memory>
#include "dr_input_defines.h"
#include <vector>
namespace driderSDK {
class MouseInput;
class HelperListener : public ::OIS::MouseListener
{
public:
  ~HelperListener() {}
  explicit HelperListener(MouseInput* pMouse);
  bool mouseMoved(const OIS::MouseEvent &arg) override;
  bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
  bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
private:
  MouseInput* m_pMouse;
};
struct DR_INPUT_EXPORT MouseInputState {
  Int32 m_pressedButtons;
  Vector2DI m_cursorPosition;
};

//Override this
class DR_INPUT_EXPORT MouseInputListener
{
public:
  virtual bool mouseMoved(const MouseInputState &state) = 0;
  virtual bool mousePressed(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
  virtual bool mouseReleased(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
};

class DR_INPUT_EXPORT MouseInput {
public:
  MouseInput(): m_helperListener(this) {}
  void Init();
  void capture();
  bool isButtonDown(MouseButtonID::E button) const;
  void setEventCallback(MouseInputListener *mouseListener);
  MouseInputState getState() const;
  std::vector<MouseInputListener*> m_listeners;
private:
  MouseInputState m_state;
  std::unique_ptr<OIS::Mouse> m_mouse;
  HelperListener m_helperListener;
};
}