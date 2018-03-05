#include "dr_web_renderer.h"
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_graphics_driver.h>
#include <dr_texture.h>
#include <dr_device.h>
#include <dr_sample_state.h>

namespace driderSDK {
CefRefPtr<DriderV8Handler> WebRenderer::m_v8Handler = new DriderV8Handler();
CefRefPtr<DriderRenderProcessHandler> WebRenderer::m_renderProcess = new DriderRenderProcessHandler();
CefRefPtr<DriderCefApp> WebRenderer::m_app = new DriderCefApp();
/*******************************************************
*                       V8
********************************************************/
bool 
DriderV8Handler::Execute(const CefString & name, 
                         CefRefPtr<CefV8Value> object, 
                         const CefV8ValueList & arguments, 
                         CefRefPtr<CefV8Value>& retval, 
                         CefString & exception)
{
  for (auto &it : m_callbacks)
  {
    if (name == it.first) {
      it.second(retval, arguments);
      return true;
    }
  }
  return false;
}

/*******************************************************
*                    RENDER PROCESS
********************************************************/
// CALLED ON RENDER PROCEESS!
void 
DriderRenderProcessHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, 
                                             CefRefPtr<CefFrame> frame, 
                                             CefRefPtr<CefV8Context> context)
{
  for (auto &it : WebRenderer::m_v8Handler->m_callbacks) {
    CefRefPtr<CefV8Value> object = context->GetGlobal();
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(it.first, WebRenderer::m_v8Handler);
    object->SetValue(it.first, func, V8_PROPERTY_ATTRIBUTE_NONE);
  }
}

bool 
DriderRenderProcessHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
                                                     CefProcessId source_process, 
                                                     CefRefPtr<CefProcessMessage> message)
{
  const std::string& message_name = message->GetName();
  if (message_name == IPC_REGISTER_JS2CPP_FUNC) {
    CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
    context->Enter();
    std::string funcName = message->GetArgumentList()->GetString(0);
    CefRefPtr<CefV8Value> object = browser->GetMainFrame()->GetV8Context()->GetGlobal();
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(funcName, WebRenderer::m_v8Handler);
    object->SetValue(funcName, func, V8_PROPERTY_ATTRIBUTE_NONE);
    context->Exit();
    return true;
  }
  return false;
}

/*******************************************************
*                        APP
********************************************************/
CefRefPtr<CefRenderProcessHandler> 
DriderCefApp::GetRenderProcessHandler()
{
  return WebRenderer::m_renderProcess;
}
/*******************************************************
*                    RENDER HANDLER
********************************************************/
bool 
RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect)
{
  rect = CefRect(0, 0, m_width, m_height);
  return true;
}
void 
RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, 
                            PaintElementType type, 
                            const RectList & dirtyRects, 
                            const void * buffer, 
                            int width, 
                            int height)
{
  m_texture->udpateFromMemory(GraphicsAPI::getDeviceContext(),
    static_cast<const char*>(buffer), width *height * 4);
}
void 
RenderHandler::init()
{
  Device& device = GraphicsAPI::getDevice();
  driderSDK::DrTextureDesc tDesc;
  tDesc.width = m_width;
  tDesc.height = m_height;
  tDesc.pitch = m_width * 4;
  tDesc.Format = DR_FORMAT::kB8G8R8A8_UNORM;
  tDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE | DR_BIND_FLAGS::RENDER_TARGET;
  tDesc.mipLevels = 0;
  tDesc.dimension = DR_DIMENSION::k2D;
  tDesc.genMipMaps = true;
  m_texture = device.createEmptyTexture(tDesc);

  driderSDK::DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_MIP_POINT;
  m_samplerState = device.createSamplerState(SSdesc);
}
void 
RenderHandler::resize(UInt32  w, UInt32  h)
{
  auto tDesc = m_texture->getDescriptor();
  m_width = w;
  m_height = h;
  tDesc.width = m_width;
  tDesc.height = m_height;
  tDesc.pitch = m_width * 4;
  m_texture->modifyTextureParams(GraphicsAPI::getDevice(), tDesc);
}

Texture* 
RenderHandler::getTexturePointer()
{
  return m_texture;
}

SamplerState* 
RenderHandler::getSamplerStatePointer()
{
  return m_samplerState;
}
/*******************************************************
*                   BROWSER CLIENT
********************************************************/
CefRefPtr<CefRenderHandler> 
BrowserClient::GetRenderHandler()
{
  return m_renderHandler;
}

bool 
BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
                                        CefProcessId source_process, 
                                        CefRefPtr<CefProcessMessage> message)
{
  return false;
}
/*******************************************************
*                    WEB RENDERER
********************************************************/
void
WebRenderer::start()
{
  CefMainArgs args;
  UInt32  result;
  //result = CefExecuteProcess(args, m_app.get(), nullptr);
  //if (result >= 0)
  //  exit(0);
  CefSettings settings;
  settings.multi_threaded_message_loop = false;
  settings.windowless_rendering_enabled = true;
  settings.single_process = true;
  settings.no_sandbox = true;
  result = CefInitialize(args, settings, m_app.get(), nullptr);
  if (!result)
    throw "CefInitialize failed";
}

void 
WebRenderer::Init(UInt32  width, UInt32  height, BROWSER_MODE::E mode)
{
  m_running = false;
  m_mode = mode;

  CefWindowInfo window_info;
  CefBrowserSettings browserSettings;
  //browserSettings.webgl = STATE_DISABLED;
  //browserSettings.plugins = STATE_DISABLED;
  //browserSettings.javascript_close_windows = STATE_DISABLED;
  //browserSettings.javascript_access_clipboard = STATE_DISABLED;
  //browserSettings.javascript_dom_paste = STATE_DISABLED;
  //browserSettings.local_storage = STATE_DISABLED;
  //browserSettings.databases = STATE_DISABLED;
  //browserSettings.universal_access_from_file_urls = STATE_DISABLED;
  //browserSettings.web_security = STATE_ENABLED;
  browserSettings.windowless_frame_rate = 60;
  browserSettings.background_color = 0;
  switch (mode)
  {
  case driderSDK::BROWSER_MODE::kHeadless:
    window_info.SetAsWindowless((HWND)GraphicsAPI::getWindowHandle());
    break;
  case driderSDK::BROWSER_MODE::kPopUp:
    window_info.SetAsPopup((HWND)GraphicsAPI::getWindowHandle(), "NAMAE");
    break;
  case driderSDK::BROWSER_MODE::kChild:
    throw "Not implemented exeption";
    break;
  }
  window_info.x = 0;
  window_info.y = 0;
  window_info.width = width;
  window_info.height = height;
  renderHandler = new RenderHandler(width, height);
  renderHandler->init();
  browserClient = new BrowserClient(renderHandler);

  browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://www.google.com", browserSettings, nullptr);
  //browser->GetMainFrame()->VisitDOM();
  startRendering();
  initInput();
}
void 
WebRenderer::Destroy()
{
  browser = nullptr;
  browserClient = nullptr;
  renderHandler = nullptr;
  CefShutdown();
}
void 
WebRenderer::startRendering()
{
  m_running = true;
}
void 
WebRenderer::stoptRendering()
{
  if (m_running) {
    m_running = false;
  }
}
void 
WebRenderer::update()
{
  if (m_running)
    CefDoMessageLoopWork();
}
void 
WebRenderer::loadURL(std::string path)
{
  browser->GetMainFrame()->LoadURL(path);
}
void 
WebRenderer::reloadPage()
{
  browser->Reload();
}
void 
WebRenderer::executeJSCode(std::string code)
{
  browser->GetMainFrame()->ExecuteJavaScript(code, 
    browser->GetMainFrame()->GetURL(), 0);
}
void 
WebRenderer::setFocus(bool focus)
{
  browser->GetHost()->SetFocus(focus);
}
void 
WebRenderer::setVisibility(bool visible)
{
  browser->GetHost()->WasHidden(!visible);
}

void WebRenderer::registerJS2CPPFunction(JSCallback callback)
{
  WebRenderer::m_v8Handler->m_callbacks.push_back(callback);
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(IPC_REGISTER_JS2CPP_FUNC);
  CefRefPtr<CefListValue> cefargs = msg->GetArgumentList();
  cefargs->SetString(0, callback.first);
  browser->SendProcessMessage(PID_RENDERER, msg);
}

void 
WebRenderer::resize(UInt32  w, UInt32  h)
{
  renderHandler->resize(w,h);
  browser->GetHost()->WasResized();
}
Texture * WebRenderer::getTexturePointer()
{
  return renderHandler->getTexturePointer();
}
Texture & WebRenderer::getTextureReference()
{
  return *renderHandler->getTexturePointer();
}
void WebRenderer::setTexture()
{
  renderHandler->getTexturePointer()->set(GraphicsAPI::getDeviceContext(), 0);
  renderHandler->getSamplerStatePointer()->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kFragment);
}
void 
WebRenderer::initInput()
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
    CefBrowserHost::DragOperationsMask opmask = DRAG_OPERATION_EVERY;
    browser->GetHost()->DragSourceEndedAt(mouseEvent.x, mouseEvent.y, opmask);
    browser->GetHost()->DragSourceSystemDragEnded();
  });
  InputManager::getKeyboard()->addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed,
    [this](KEY_CODE::E key)
  {
    CefKeyEvent keyEvent;
    keyEvent.modifiers =  0; // InputManager::instance().getKeyboard().;
    char c = Keyboard::getAsChar(key);
    keyEvent.windows_key_code = (int) c;
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    browser->GetHost()->SendKeyEvent(keyEvent);

    keyEvent.type = KEYEVENT_CHAR;
    browser->GetHost()->SendKeyEvent(keyEvent);
  });
}
}