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
namespace driderSDK {
  

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
    //std::cout << "paint" << std::endl;
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

class DR_UI_EXPORT WebRenderer {
public:
  static void start() {
    CefMainArgs args;
    int result = CefExecuteProcess(args, nullptr, nullptr);
    if (result >= 0)
      throw "CEF_INITIALIZATION FAILED: CefExecuteProcess";
    CefSettings settings;
    settings.multi_threaded_message_loop = false;
    settings.windowless_rendering_enabled = true;
    //settings.no_sandbox = true;
    result = CefInitialize(args, settings, nullptr, nullptr);
    if (!result)
      throw "CEF_INITIALIZATION FAILED: CefInitialize";
  }
  WebRenderer(){}
  void Init(GraphicsAPI* api,Quad* quad );
  void Destroy();
  void captureInput();
  void startRendering();
  void stoptRendering();
  void update();
  void loadURL(std::string path);
private:
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browserClient;
  CefRefPtr<RenderHandler> renderHandler;
  bool m_running;
  Quad* m_quad;
  GraphicsAPI* m_api;
};
}