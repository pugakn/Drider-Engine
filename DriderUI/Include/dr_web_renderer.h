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
namespace driderSDK {
namespace BROWSER_MODE {
  enum E {
    kHeadless,
    kPopUp,
    kChild
  };
}
typedef std::map<std::pair<std::string, int>,
  std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>>
  CallbackMap;

class DriderV8Handler : public CefV8Handler {
public:
  DriderV8Handler() {}

  virtual bool Execute(const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception) override {
    if (name == "myfunc") {
      // Return my string value.
      retval = CefV8Value::CreateString("My Value!");
      return true;
    }

    // Function does not exist.
    return false;
  }
  IMPLEMENT_REFCOUNTING(DriderV8Handler);
};
class RenderHandler: public CefRenderHandler
{
public:
  RenderHandler( Quad* quad,int w,int h): 
    m_width(w), m_height(h), m_renderQuad(quad){}
  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
  void OnPaint(CefRefPtr<CefBrowser> browser, 
               PaintElementType type, 
               const RectList &dirtyRects, 
               const void *buffer, 
               int width, 
               int height) override;
  void resize(int  w, int h);

  IMPLEMENT_REFCOUNTING(RenderHandler);
private:
  int m_width;
  int m_height;
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



class DR_UI_EXPORT WebRenderer {
public:
  static void start();
  WebRenderer(){}
  void Init(Quad* quad, BROWSER_MODE::E mode = BROWSER_MODE::kHeadless);
  void Destroy();
  void startRendering();
  void stoptRendering();
  void update();

  void loadURL(std::string path);
  void reloadPage();
  void executeJSCode(std::string code);

  void setFocus(bool focus);
  void setVisibility(bool visible);

  void resize(int w, int h);


private:
  void initInput();
  CallbackMap callback_map_;
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefRefPtr<RenderHandler> renderHandler;
  bool m_running;
  Quad* m_quad;
  BROWSER_MODE::E m_mode;
};
}