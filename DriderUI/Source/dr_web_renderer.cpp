#include "dr_web_renderer.h"
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_graphics_driver.h>
#include <dr_texture.h>

#ifdef DR_PLATFORM_WINDOWS
#include <dr_d3d_device.h>
#include <dr_d3d_sample_state.h>
#endif
namespace driderSDK {
CefRefPtr<DriderRenderProcessHandler> WebRenderer::m_renderProcess = new DriderRenderProcessHandler();
CefRefPtr<DriderCefApp> WebRenderer::m_app = new DriderCefApp(m_renderProcess);
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
  m_texture->udpateFromMemory(GraphicsDriver::getApiReference().getDeviceContextReference(),
    static_cast<const char*>(buffer), width *height * 4);
}
void RenderHandler::init()
{
  Device& device = GraphicsDriver::getApiReference().getDeviceReference();
  driderSDK::DrTextureDesc tDesc;
  tDesc.width = m_width;
  tDesc.height = m_height;
  tDesc.pitch = m_width * 4;
  tDesc.Format = DR_FORMAT::kDrFormat_B8G8R8A8_UNORM;
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
RenderHandler::resize(int w, int h)
{
  auto tDesc = m_texture->getDescriptor();
  m_width = w;
  m_height = h;
  tDesc.width = m_width;
  tDesc.height = m_height;
  tDesc.pitch = m_width * 4;
  m_texture->modifyTextureParams(GraphicsDriver::getApiReference().getDeviceReference(), tDesc);
}

Texture * RenderHandler::getTexturePointer()
{
  return m_texture;
}

SamplerState * RenderHandler::getSamplerStatePointer()
{
  return m_samplerState;
}

void 
WebRenderer::start()
{
  CefMainArgs args;
  int result = CefExecuteProcess(args, m_app.get(), nullptr);
  if (result >= 0)
    exit(0);
  CefSettings settings;
  settings.multi_threaded_message_loop = false;
  settings.windowless_rendering_enabled = true;
  settings.single_process = false;
  settings.no_sandbox = true;
  result = CefInitialize(args, settings, nullptr, nullptr);
  if (!result)
    throw "CefInitialize failed";
}

/*******************************************************
*                    WEB RENDERER
********************************************************/
void 
WebRenderer::Init(int width, int height, BROWSER_MODE::E mode)
{
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
  window_info.width = width;
  window_info.height = height;
  renderHandler = new RenderHandler(width, height);
  renderHandler->init();
  browserClient = new BrowserClient(renderHandler);

  browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://www.google.com", browserSettings, nullptr);
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
  browserClient->m_callbacks.push_back(callback);
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(IPC_REGISTER_JS2CPP_FUNC);
  CefRefPtr<CefListValue> cefargs = msg->GetArgumentList();
  cefargs->SetString(0, callback.first);
  browser->SendProcessMessage(PID_RENDERER, msg);
}

void 
WebRenderer::resize(int w, int h)
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
  renderHandler->getTexturePointer()->set(GraphicsDriver::getApiReference().getDeviceContextReference(), 0);
  renderHandler->getSamplerStatePointer()->set(GraphicsDriver::getApiReference().getDeviceContextReference(), DR_SHADER_TYPE_FLAG::kFragment);
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
    //keyEvent.modifiers =  0; // InputManager::instance().getKeyboard().;
    keyEvent.windows_key_code = key;
    keyEvent.type = KEYEVENT_RAWKEYDOWN;
    browser->GetHost()->SendKeyEvent(keyEvent);

    keyEvent.type = KEYEVENT_CHAR;
    browser->GetHost()->SendKeyEvent(keyEvent);
  });
}

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  const std::string& message_name = message->GetName();
  if (message_name == IPC_CALL_JS2CPP_FUNC) {
    std::string funcName = message->GetArgumentList()->GetString(0);
    for (auto &it : m_callbacks)
    {
      if (funcName == it.first) {
        CefRefPtr<CefV8Value> retval;
        for (size_t i = 1; i < message->GetArgumentList()->GetSize(); i++)
        {
          if (message->GetArgumentList()->GetType(i) == CefValueType::VTYPE_INT ) {

          }
          else if (message->GetArgumentList()->GetType(i) == CefValueType::VTYPE_BOOL)
          {

          }
          else if (message->GetArgumentList()->GetType(i) == CefValueType::VTYPE_DOUBLE)
          {

          }
          else if (message->GetArgumentList()->GetType(i) == CefValueType::VTYPE_STRING)
          {

          }
        }
        
        it.second(retval);
        return true;
      }
    }
    return true;
  }
  return false;
}

// CALLED ON RENDER PROCEESS!
void DriderRenderProcessHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  for (auto &it : m_v8Handler->m_callList) {
    CefRefPtr<CefV8Value> object = context->GetGlobal();
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(it, m_v8Handler);
    object->SetValue(it, func, V8_PROPERTY_ATTRIBUTE_NONE);
  }
}

bool DriderRenderProcessHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  const std::string& message_name = message->GetName();
  if (message_name == IPC_REGISTER_JS2CPP_FUNC) {
    CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
    context->Enter();
    std::string funcName = message->GetArgumentList()->GetString(0);
    m_v8Handler->m_callList.push_back(funcName);
    CefRefPtr<CefV8Value> object = browser->GetMainFrame()->GetV8Context()->GetGlobal();
    CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(funcName, m_v8Handler);
    object->SetValue(funcName, func, V8_PROPERTY_ATTRIBUTE_NONE);
    context->Exit();
    return true;
  }
  return false;
}

}