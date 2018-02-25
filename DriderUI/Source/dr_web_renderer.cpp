#include "dr_web_renderer.h"
#include"dr_input_manager.h"
#include "dr_keyboard.h"
#include "dr_mouse.h"
#include "dr_graphics_driver.h"
namespace driderSDK {
/*******************************************************
*                    RENDER HANDLER
********************************************************/
bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect)
{
  rect = CefRect(0, 0, m_width, m_height);
  return true;
}
void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, 
                            PaintElementType type, 
                            const RectList & dirtyRects, 
                            const void * buffer, 
                            int width, 
                            int height)
{
  m_renderQuad->texture->udpateFromMemory(GraphicsDriver::getApiReference().getDeviceContextReference(),
    static_cast<const char*>(buffer), width *height * 4);
}
void RenderHandler::resize(int w, int h)
{
  m_width = w;
  m_height = h;
  //m_renderQuad->texture.resize(w,h);
}

void WebRenderer::start()
{
  CefMainArgs args;
  int result = CefExecuteProcess(args, nullptr, nullptr);
  if (result >= 0)
    exit(0);
  CefSettings settings;
  settings.multi_threaded_message_loop = false;
  settings.windowless_rendering_enabled = true;
  settings.single_process = true;
  settings.no_sandbox = true;
  result = CefInitialize(args, settings, nullptr, nullptr);
  if (!result)
    throw "CefInitialize failed";
}

/*void OnContextCreated(
CefRefPtr<CefBrowser> browser,
CefRefPtr<CefFrame> frame,
CefRefPtr<CefV8Context> context)override {

CefRefPtr<CefV8Value> object = context->GetGlobal();

// Create an instance of my CefV8Handler object.
CefRefPtr<CefV8Handler> handler = new DriderV8Handler();

// Create the "myfunc" function.
CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("myfunc", handler);

// Add the "myfunc" function to the "window" object.
object->SetValue("myfunc", func, V8_PROPERTY_ATTRIBUTE_NONE);
}*/
  /*******************************************************
  *                    WEB RENDERER
  ********************************************************/
  void WebRenderer::Init(Quad* quad, BROWSER_MODE::E mode)
  {
     m_quad = quad;
     m_running = false;
     m_mode = mode;

     CefWindowInfo window_info;
     CefBrowserSettings browserSettings;
     browserSettings.webgl = STATE_DISABLED;
     browserSettings.plugins = STATE_DISABLED;
     browserSettings.javascript_close_windows = STATE_DISABLED;
     browserSettings.javascript_access_clipboard = STATE_DISABLED;
     browserSettings.javascript_dom_paste = STATE_DISABLED;
     browserSettings.local_storage = STATE_DISABLED;
     browserSettings.databases = STATE_DISABLED;
     browserSettings.universal_access_from_file_urls = STATE_DISABLED;
     browserSettings.web_security = STATE_ENABLED;
     browserSettings.windowless_frame_rate = 60;
     
     switch (mode)
     {
     case driderSDK::BROWSER_MODE::kHeadless:
       window_info.SetAsWindowless((HWND)GraphicsDriver::getApiReference().getWindowHandler());
       break;
     case driderSDK::BROWSER_MODE::kPopUp:
       window_info.SetAsPopup((HWND)GraphicsDriver::getApiReference().getWindowHandler(), "NAMAE");
       break;
     case driderSDK::BROWSER_MODE::kChild:
       throw "Not implemented exeption";
       break;
     }
     window_info.x = 0;
     window_info.y = 0;
     window_info.width = quad->width;
     window_info.height = quad->height;
     renderHandler = new RenderHandler( m_quad,quad->width, quad->height);
     browserClient = new BrowserClient(renderHandler);

     browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://www.google.com", browserSettings, nullptr);//http://ich.deanmcnamee.com/pre3d/monster.html
     startRendering();
     initInput();
  }
  void WebRenderer::Destroy()
  {
    browser = nullptr;
    browserClient = nullptr;
    renderHandler = nullptr;
    CefShutdown();
  }
  void WebRenderer::startRendering()
  {
    m_running = true;
  }
  void WebRenderer::stoptRendering()
  {
    if (m_running) {
      m_running = false;
    }
  }
  void WebRenderer::update()
  {
    if (m_running)
      CefDoMessageLoopWork();
  }
  void WebRenderer::loadURL(std::string path)
  {
    browser->GetMainFrame()->LoadURL(path);
  }
  void WebRenderer::reloadPage()
  {
    browser->Reload();
  }
  void WebRenderer::executeJSCode(std::string code)
  {
    browser->GetMainFrame()->ExecuteJavaScript(code, 
      browser->GetMainFrame()->GetURL(), 0);
  }
  void WebRenderer::setFocus(bool focus)
  {
    browser->GetHost()->SetFocus(focus);
  }
  void WebRenderer::setVisibility(bool visible)
  {
    browser->GetHost()->WasHidden(!visible);
  }
  void WebRenderer::resize(int w, int h)
  {
    renderHandler->resize(w,h);
    browser->GetHost()->WasResized();
  }
  void WebRenderer::initInput()
  {
    InputManager::getMouse()->addMovedCallback([this]()
    {
      auto pos = Mouse::getPosition();
      CefMouseEvent mouseEvent;
      mouseEvent.x = pos.x;
      mouseEvent.y = pos.y;
      browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
    });
    InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed,
      [this](MOUSE_BUTTON::E pressedId)
    {
      auto pos = Mouse::getPosition();
      CefMouseEvent mouseEvent;
      mouseEvent.x = pos.x;
      mouseEvent.y = pos.y;
      cef_mouse_button_type_t button;
      if (pressedId == MOUSE_BUTTON::kRight) {
        button = MBT_RIGHT;
      }
      if (pressedId == MOUSE_BUTTON::kLeft) {
        button = MBT_LEFT;
      }
      if (pressedId == MOUSE_BUTTON::kMiddle) {
        button = MBT_MIDDLE;
      }
      browser->GetHost()->SendMouseClickEvent(mouseEvent, button, false, 1);
    });
    InputManager::getMouse()->addAnyButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased,
      [this](MOUSE_BUTTON::E pressedId)
    {
      auto pos = Mouse::getPosition();
      CefMouseEvent mouseEvent;
      mouseEvent.x = pos.x;
      mouseEvent.y = pos.y;
      cef_mouse_button_type_t button;
      if (pressedId == MOUSE_BUTTON::kRight) {
        button = MBT_RIGHT;
      }
      if (pressedId == MOUSE_BUTTON::kLeft) {
        button = MBT_LEFT;
      }
      if (pressedId == MOUSE_BUTTON::kMiddle) {
        button = MBT_MIDDLE;
      }
      browser->GetHost()->SendMouseClickEvent(mouseEvent, button, true, 1);
    });
    InputManager::getKeyboard()->addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
      [this](KEY_CODE::E key)
    {
      CefKeyEvent keyEvent;
      //keyEvent.modifiers =  0; // InputManager::instance().getKeyboard().;
      keyEvent.windows_key_code = key;
      keyEvent.type = KEYEVENT_RAWKEYDOWN;
      browser->GetHost()->SendKeyEvent(keyEvent);

      keyEvent.type = KEYEVENT_CHAR;
      browser->GetHost()->SendKeyEvent(keyEvent);
    });
  }
}