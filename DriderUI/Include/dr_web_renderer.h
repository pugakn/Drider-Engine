#pragma once
#include "dr_ui_prerequisites.h"
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_life_span_handler.h>
#include <include/cef_load_handler.h>
#include <include/wrapper/cef_helpers.h>

#include <dr_graphics_api.h>
#include <dr_quad.h>
#include <memory>
#include <iostream>

#include"dr_input_manager.h"
#include "dr_input_keyboard.h"
#include "dr_input_mouse.h"
namespace driderSDK {
typedef std::map<std::pair<std::string, int>,
  std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > >
  CallbackMap;
class RenderHandler: public CefRenderHandler
{
public:
  RenderHandler(int w,int h)
  {
    width = w;
    height = h;
  }
  void init(GraphicsAPI* api, Quad* quad) {
    m_renderQuad = quad;
    m_graphicsApi = api;
  }
  // CefRenderHandler interface
  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
  {
    rect = CefRect(0, 0, width, height);
    return true;
  }
  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override
  {
    m_renderQuad->texture->udpateFromMemory(*m_graphicsApi->deviceContext,(char*)buffer, width *height*4);
  }
  void 
  OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
    double x,
    double y) override{
    //CefMouseEvent event;
    //browser->GetHost()->SendMouseWheelEvent(event, x, y);
  }

  IMPLEMENT_REFCOUNTING(RenderHandler);
private:
  int width;
  int height;
  size_t m_hwnd;
  Quad* m_renderQuad;
  GraphicsAPI* m_graphicsApi;
};
class BrowserClient : public CefClient
{
public:
  BrowserClient(RenderHandler *renderHandler) : m_renderHandler(renderHandler) {}
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
    return m_renderHandler;
  }
  CefRefPtr<CefRenderHandler> m_renderHandler;
  IMPLEMENT_REFCOUNTING(BrowserClient);
};

class DR_UI_EXPORT WebRenderer : public driderSDK::IKeyboardListener,  driderSDK::IMouseInputListener {
public:
  static void start() {
    CefMainArgs args;
    int result = CefExecuteProcess(args, nullptr, nullptr);
    std::cout << "res: " << result << std::endl;
    if (result >= 0)
      exit(0);
    CefSettings settings;
    settings.multi_threaded_message_loop = false;
    settings.windowless_rendering_enabled = true;
    settings.single_process = true;
    settings.no_sandbox = true;
    result = CefInitialize(args, settings, nullptr, nullptr);
    if (!result)
      throw "CEF_INITIALIZATION FAILED: CefInitialize";
  }
  WebRenderer(){}
  void Init(GraphicsAPI* api,Quad* quad );
  void Destroy();
  void startRendering();
  void stoptRendering();
  void update();
  void loadURL(std::string path);


  //Input
  bool
  keyPressed(const KeyboardButtonID::E& key) override {
    CefKeyEvent keyEvent;
    if (key == KeyboardButtonID::KC_Q) {
    }
    if (key == KeyboardButtonID::KC_A) {
    }
    if (key == KeyboardButtonID::KC_1) {
    }

    //keyEvent.modifiers =  0; // InputManager::instance().getKeyboard().;
    keyEvent.windows_key_code =  key;
    //keyEvent.
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    browser->GetHost()->SendKeyEvent(keyEvent);

    keyEvent.type = KEYEVENT_CHAR;
    browser->GetHost()->SendKeyEvent(keyEvent);
    return true;
  }

  bool
  keyReleased(const KeyboardButtonID::E& key) override {
    return false;
  }


  //Mouse
  bool
  mouseMoved(const MouseInputState& state) override {
  CefMouseEvent mouseEvent;
  mouseEvent.x = state.cursorPosition.x;
  mouseEvent.y = state.cursorPosition.y;
  browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
  return true;
  }

  bool
  mousePressed(const MouseInputState& state, MouseButtonID::E pressedId) override {
  CefMouseEvent mouseEvent;
  mouseEvent.x = state.cursorPosition.x;
  mouseEvent.y = state.cursorPosition.y;
  cef_mouse_button_type_t button;
  if (pressedId == MouseButtonID::MB_Right) {
    button = MBT_RIGHT;
  }
  if (pressedId == MouseButtonID::MB_Left) {
    button = MBT_LEFT;
  }
  if (pressedId == MouseButtonID::MB_Middle) {
    button = MBT_MIDDLE;
  }
  browser->GetHost()->SendMouseClickEvent(mouseEvent, button ,false,1);
  return true;
  }

  bool
  mouseReleased(const MouseInputState& state, MouseButtonID::E pressedId) override {
  CefMouseEvent mouseEvent;
  mouseEvent.x = state.cursorPosition.x;
  mouseEvent.y = state.cursorPosition.y;
  cef_mouse_button_type_t button;
  if (pressedId == MouseButtonID::MB_Right) {
    button = MBT_RIGHT;
  }
  if (pressedId == MouseButtonID::MB_Left) {
    button = MBT_LEFT;
  }
  if (pressedId == MouseButtonID::MB_Middle) {
    button = MBT_MIDDLE;
  }
  browser->GetHost()->SendMouseClickEvent(mouseEvent, button, true, 1);
  return true;
  }

private:

  CallbackMap callback_map_;
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefRefPtr<RenderHandler> renderHandler;
  bool m_running;
  Quad* m_quad;
  GraphicsAPI* m_api;
};
}