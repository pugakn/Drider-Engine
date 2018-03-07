#pragma once
#include "dr_ui_prerequisites.h"
#include <dr_gfx_memory.h>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_life_span_handler.h>
#include <include/cef_load_handler.h>
#include <include/wrapper/cef_helpers.h>

#include <vector>
#include <functional>
#include <iostream>
namespace driderSDK {
class Texture;
class SamplerState;
static const std::string IPC_REGISTER_JS2CPP_FUNC = "ipc01";
static const std::string IPC_CALL_JS2CPP_FUNC     = "ipc00";
namespace BROWSER_MODE {
enum E {
  kHeadless,
  kPopUp,
  kChild
};
}
using JSCallLambda = std::function<void(CefRefPtr<CefV8Value>&, const CefV8ValueList&)>;
using JSCallback = std::pair<std::string, JSCallLambda>;
using JSCalls = std::vector<JSCallback>; 

/**
* Internal V8 handler
*
* This class manages the JS <-> C++ communication
*/
class DriderV8Handler : public CefV8Handler {
 public:
  DriderV8Handler() = default;
  DriderV8Handler(const DriderV8Handler&) = delete;
  DriderV8Handler(DriderV8Handler&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderV8Handler);
 private:
  friend class WebRenderer;
  friend class DriderRenderProcessHandler;
  /**
  * Not commented
  */
  virtual bool 
  Execute(const CefString& name,
          CefRefPtr<CefV8Value> object,
          const CefV8ValueList& arguments,
          CefRefPtr<CefV8Value>& retval,
          CefString& exception) override;

  JSCalls m_callbacks;
};

/**
* Internal render process handler
*
* This class manages all the communication with the 
* render process from the browser process
*/
class DriderRenderProcessHandler : public  CefRenderProcessHandler {
 public:
  DriderRenderProcessHandler() = default;
  DriderRenderProcessHandler(const DriderRenderProcessHandler&) = delete;
  DriderRenderProcessHandler(DriderRenderProcessHandler&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderRenderProcessHandler);
 private:
  friend class WebRenderer;
  /**
  * Not commented
  */
  void 
  OnContextCreated(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   CefRefPtr<CefV8Context> context) override;
  /**
  * Not commented
  */
  bool 
  OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                           CefProcessId source_process,
                           CefRefPtr<CefProcessMessage> message) override;

};

/**
* Internal application event handler
*/
class DriderCefApp : public CefApp {
 public:
  DriderCefApp() = default;
  DriderCefApp(const DriderCefApp&) = delete;
  DriderCefApp(DriderCefApp&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderCefApp);
 private:
  friend class WebRenderer;
  /**
  * Not commented
  */
  virtual CefRefPtr<CefRenderProcessHandler> 
  GetRenderProcessHandler();
};

/**
* Internal render handler
*
* This class manages all the headless rendering
* and events related with the render area.
*/
class RenderHandler: public CefRenderHandler
{
 public:
  RenderHandler(const RenderHandler&) = delete;
  RenderHandler(RenderHandler&&) = delete;
  IMPLEMENT_REFCOUNTING(RenderHandler);
 private:
  friend class WebRenderer;
  /**
  * Not commented
  */
  RenderHandler(UInt32  w, UInt32  h) :
                m_width(w),
                m_height(h) {}
  /**
  * Not commented
  */
  void init();
  /**
  * Not commented
  */
  bool 
  GetViewRect(CefRefPtr<CefBrowser> browser, 
              CefRect &rect) override;
  /**
  * Not commented
  */
  void 
  resize(UInt32  w, UInt32 h);
  /**
  * Not commented
  */
  const Texture& getTexture();
  /**
  * Not commented
  */
  const SamplerState& getSamplerState();
  /**
  * Not commented
  */
  void
  OnPaint(CefRefPtr<CefBrowser> browser,
          PaintElementType type,
          const RectList &dirtyRects,
          const void *buffer,
          int width,
          int height) override;
  /**
  * Not commented
  */
  bool StartDragging(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefDragData> drag_data,
                     DragOperationsMask allowed_ops,
                     UInt32  x,
                     UInt32  y) {
                      m_dragging = true;
                      return true;
                     }


  bool m_dragging = false;
  UInt32  m_width;
  UInt32  m_height;
  size_t m_hwnd;
  GFXUnique<Texture> m_texture;
  GFXUnique<SamplerState>  m_samplerState;
};


/**
* Internal browser client
*
* This class is used to provide the render hanler
* to the process, and catch the IPC messages for
* the browser process.
*/
class BrowserClient : public CefClient
{
 public:
  BrowserClient(const BrowserClient&) = delete;
  BrowserClient(BrowserClient&&) = delete;
  IMPLEMENT_REFCOUNTING(BrowserClient);
 private:
  friend class WebRenderer;
  explicit BrowserClient(RenderHandler *renderHandler) : 
                         m_renderHandler(renderHandler) {}
  /**
  * Not commented
  */
  virtual CefRefPtr<CefRenderHandler>
  GetRenderHandler();
  /**
  * Not commented
  */
  virtual bool 
  OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                           CefProcessId source_process,
                           CefRefPtr<CefProcessMessage> message);

  CefRefPtr<CefRenderHandler> m_renderHandler;
};


/**
* Web render manager
*
* This class is used to manage all the web render,
* events, and settings for the browser.
* 
* You must create one WebRenderer for each render area you want.
*/
class DR_UI_EXPORT WebRenderer {
 public:
  /**
  * Call it once before everything on the main function
  * It initializes the necesary process to run chromium
  */
  static void start();
  WebRenderer(){}
  void 
  Init(UInt32 width, UInt32 height, BROWSER_MODE::E mode = BROWSER_MODE::kHeadless);
  /**
  * Free the allocated memory
  */
  void 
  Destroy();
  /**
  * Set m_running to true
  */
  void 
  startRendering();
  /**
  * Set m_running to false
  */
  void 
  stoptRendering();
  /**
  * Do a render loop work
  */
  void 
  update();
  /**
  * Not commented
  */
  void 
  loadURL(std::string path);
  /**
  * Not commented
  */
  void 
  reloadPage();
  /**
  * Not commented
  */
  void 
  executeJSCode(std::string code);
  /**
  * Not commented
  */
  void 
  setFocus(bool focus);
  /**
  * Not commented
  */
  void 
  setVisibility(bool visible);
  /**
  * Not commented
  */
  void 
  registerJS2CPPFunction(JSCallback callback);

  /* Only on Headless context */

  /**
  * Resize the render area
  *
  * @param w
  *  Width
  *
  * @param h
  *  Height
  *
  */
  void
  resize(UInt32  w, UInt32 h);

  /**
  * Not commented
  */
  const Texture& getTexture();
  /**
  * Not commented
  */
  void setTexture();

  //CefRefPtr<CefV8Value> getJSGlobalVar(std::string name) {
  //  //CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
  //  CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
  //  context->Enter();
  //  auto window = context->GetGlobal();
  //  CefRefPtr<CefV8Value> retVal  = window->GetValue(name);
  //  context->Exit();
  //  return retVal;
  //}

  //CefRefPtr<CefV8Value> executeJSFunction(std::string name, const CefV8ValueList& args) {
  //  CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
  //  context->Enter();
  //  auto window = context->GetGlobal();
  //  auto func = window->GetValue(name);
  //  CefRefPtr<CefV8Value> retVal = func->ExecuteFunction(window, args);
  //  context->Exit();
  //  return retVal;
  //}

 private:
  friend class DriderCefApp;
  friend class DriderRenderProcessHandler;
  static CefRefPtr<DriderCefApp> m_app;
  static CefRefPtr<DriderRenderProcessHandler> m_renderProcess;
  static CefRefPtr<DriderV8Handler> m_v8Handler;
  void 
  initInput();
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefRefPtr<RenderHandler> renderHandler;
  bool m_running;
  BROWSER_MODE::E m_mode;
};
}