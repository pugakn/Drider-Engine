#pragma once
#include "dr_ui_prerequisites.h"
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_life_span_handler.h>
#include <include/cef_load_handler.h>
#include <include/wrapper/cef_helpers.h>


namespace driderSDK {
class Texture;
class SamplerState;
using CallbackMap = std::map<std::pair<std::string, int>,
  std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>>;

namespace BROWSER_MODE {
enum E {
  kHeadless,
  kPopUp,
  kChild
};
}

class DriderV8Handler : public CefV8Handler {
public:
  DriderV8Handler() {}
  virtual bool Execute(const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception) override {
    if (name == "myfunc") {
      retval = CefV8Value::CreateString("My Value!");
      return true;
    }
    return false;
  }
  IMPLEMENT_REFCOUNTING(DriderV8Handler);
};

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
  void 
  init();

  int m_width;
  int m_height;
  size_t m_hwnd;
  Texture* m_texture;
  SamplerState* m_samplerState;
};


class BrowserClient : public CefClient
{
public:
  BrowserClient(const BrowserClient&) = delete;
  BrowserClient(BrowserClient&&) = delete;
  IMPLEMENT_REFCOUNTING(BrowserClient);
private:
  friend class WebRenderer;
  BrowserClient(RenderHandler *renderHandler) : 
    m_renderHandler(renderHandler) {}

  virtual CefRefPtr<CefRenderHandler> 
  GetRenderHandler() {
    return m_renderHandler;
  }
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

  /* Only on Headless context */
  Texture* getTexturePointer();
  Texture& getTextureReference();

  void setTexture();
private:
  void 
  initInput();
  CallbackMap callback_map_;
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefRefPtr<RenderHandler> renderHandler;
  bool m_running;
  BROWSER_MODE::E m_mode;
};
}