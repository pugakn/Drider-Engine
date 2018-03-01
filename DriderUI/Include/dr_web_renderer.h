#pragma once
#include "dr_ui_prerequisites.h"
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

class DriderV8Handler : public CefV8Handler {
public:
  DriderV8Handler() = default;
  DriderV8Handler(const DriderV8Handler&) = delete;
  DriderV8Handler(DriderV8Handler&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderV8Handler);
private:
  friend class WebRenderer;
  friend class DriderRenderProcessHandler;
  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) override;

  JSCalls m_callbacks;
};
class DriderRenderProcessHandler : public  CefRenderProcessHandler {
public:
  DriderRenderProcessHandler() = default;
  DriderRenderProcessHandler(const DriderRenderProcessHandler&) = delete;
  DriderRenderProcessHandler(DriderRenderProcessHandler&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderRenderProcessHandler);
private:
  friend class WebRenderer;

  void OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) override;

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) override;

};
class DriderCefApp : public CefApp {
public:
  DriderCefApp() = default;
  DriderCefApp(const DriderCefApp&) = delete;
  DriderCefApp(DriderCefApp&&) = delete;
  IMPLEMENT_REFCOUNTING(DriderCefApp);
private:
  friend class WebRenderer;
  virtual CefRefPtr<CefRenderProcessHandler> 
  GetRenderProcessHandler();

};

/**
* Internal render handler
*/
class RenderHandler: public CefRenderHandler
{
public:
  RenderHandler(const RenderHandler&) = delete;
  RenderHandler(RenderHandler&&) = delete;
  IMPLEMENT_REFCOUNTING(RenderHandler);
private:
  friend class WebRenderer;

  bool 
  GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
  void 
  resize(UInt32  w, UInt32 h);
  Texture* getTexturePointer();
  SamplerState* getSamplerStatePointer();

  RenderHandler(UInt32  w, UInt32  h) :
    m_width(w),
    m_height(h) {}

  void
  OnPaint(CefRefPtr<CefBrowser> browser,
          PaintElementType type,
          const RectList &dirtyRects,
          const void *buffer,
          int width,
          int height) override;

  bool StartDragging(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> drag_data,
    DragOperationsMask allowed_ops,
    UInt32  x,
    UInt32  y) {
    m_dragging = true;
    return true;
  }
  void 
  init();


  bool m_dragging = false;
  UInt32  m_width;
  UInt32  m_height;
  size_t m_hwnd;
  Texture* m_texture;
  SamplerState* m_samplerState;
};


/**
* Internal browser client
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
  virtual CefRefPtr<CefRenderHandler>
  GetRenderHandler();

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message);

  CefRefPtr<CefRenderHandler> m_renderHandler;
};



class DR_UI_EXPORT WebRenderer {
public:
  static void start();
  WebRenderer(){}
  void 
  Init(UInt32 width, UInt32 height, BROWSER_MODE::E mode = BROWSER_MODE::kHeadless);
  void 
  Destroy();
  void 
  startRendering();
  void 
  stoptRendering();
  void 
  update();

  void 
  loadURL(std::string path);
  void 
  reloadPage();
  void 
  executeJSCode(std::string code);

  void 
  setFocus(bool focus);
  void 
  setVisibility(bool visible);

  void registerJS2CPPFunction(JSCallback callback);

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
  Texture* getTexturePointer();
  Texture& getTextureReference();
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