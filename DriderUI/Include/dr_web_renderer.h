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
/*RENDER PROCESS OBJECTS*/
using JSCallLambda = void(*)(CefRefPtr<CefV8Value>& retval, const CefV8ValueList&);
using JSCallback = std::pair<std::string, JSCallLambda>; //Verctor de maps std::function<void(CefRefPtr<CefV8Value>&)>
using JSCalls = std::vector<JSCallback>; //vector<JSCalls<Args...>>

class DriderV8Handler : public CefV8Handler {
public:
  JSCalls m_callbacks;
  DriderV8Handler() {}
  virtual bool Execute(const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception) override {
    for (auto &it : m_callbacks)
    {
    if (name == it.first) {
        it.second(retval,arguments);
      }
    }
    //for (auto &it : m_callList)
    //{
    //  if (name == it) {
    //    //retval = CefV8Value::CreateString("Waaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    //    CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
    //    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(IPC_CALL_JS2CPP_FUNC);
    //    CefRefPtr<CefListValue> cefargs = msg->GetArgumentList();
    //    cefargs->SetString(0, name);
    //    int idNum = 1;
    //    for (auto&it : arguments) {
    //      if (it->IsInt() || it->IsUInt()) {
    //        cefargs->SetInt(idNum++,it->GetIntValue());
    //      }else
    //      if (it->IsBool()) {
    //        cefargs->SetBool(idNum++, it->GetBoolValue());
    //      }else
    //      if (it->IsDouble()) {
    //        cefargs->SetDouble(idNum++, it->GetDoubleValue());
    //      }else
    //      if (it->IsString()) {
    //        cefargs->SetString(idNum++, it->GetStringValue());
    //      }
    //    }
    //    //browser->SendProcessMessage(PID_BROWSER, msg);


    //    //context->Enter();
    //    //auto window = context->GetGlobal();
    //    //auto bnfront = window->GetValue("bnfront");
    //    //bnfront->ExecuteFunction();
    //    //context->Exit();
    //    //CefRefPtr<CefV8Context> context = browser->GetMainFrame()->GetV8Context();
    //    //context->Enter();
    //    //auto window = context->GetGlobal();
    //    //auto bnfront = window->GetValue("testVal01");
    //    //std::cout << " VALsadsdsd: " << bnfront->GetIntValue() << std::endl;
    //    //CefRefPtr<CefV8Value> newVal= CefV8Value::CreateInt(52);
    //    //bnfront->SetValue("testVal01",newVal, V8_PROPERTY_ATTRIBUTE_NONE);
    //    //bnfront = window->GetValue("testVal01");
    //    //std::cout << " XX: " << bnfront->GetIntValue() << std::endl;
    //    //context->Exit();
    //    return true;
    //  }
    //}
    return false;
  }
  //std::vector<std::string> m_callList;
  IMPLEMENT_REFCOUNTING(DriderV8Handler);
};
class DriderRenderProcessHandler : public  CefRenderProcessHandler {
private:
  friend class WebRenderer;
  explicit DriderRenderProcessHandler(DriderV8Handler* handler) { m_v8Handler = handler; }
  void OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) override;

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) override;

  CefRefPtr<DriderV8Handler> m_v8Handler;
  IMPLEMENT_REFCOUNTING(DriderRenderProcessHandler);
};
class DriderCefApp : public CefApp {
private:
  friend class WebRenderer;
  explicit DriderCefApp(DriderRenderProcessHandler* renderProcess) : m_processHandler(renderProcess){}
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() {
    return m_processHandler;
  }
  CefRefPtr<DriderRenderProcessHandler> m_processHandler;
  IMPLEMENT_REFCOUNTING(DriderCefApp);
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
  bool 
  GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
  void 
  resize(int  w, int h);
  Texture* getTexturePointer();
  SamplerState* getSamplerStatePointer();

  friend class WebRenderer;
  RenderHandler(int w, int h) :
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
    int x,
    int y) {
    m_dragging = true;
    return true;
  }
  void 
  init();

  bool m_dragging = false;
  int m_width;
  int m_height;
  size_t m_hwnd;
  Texture* m_texture;
  SamplerState* m_samplerState;
};



/*BROWSER PROCESS OBJECTS*/
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
    m_renderHandler(renderHandler) {
  }

  virtual CefRefPtr<CefRenderHandler> 
  GetRenderHandler() {
    return m_renderHandler;
  }

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
  Init(int width, int height, BROWSER_MODE::E mode = BROWSER_MODE::kHeadless);
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
    resize(int  w, int h);
  Texture* getTexturePointer();
  Texture& getTextureReference();
  void setTexture();

  template<class T>
  T getGlobalVar(std::string name) {
    
  }

private:
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