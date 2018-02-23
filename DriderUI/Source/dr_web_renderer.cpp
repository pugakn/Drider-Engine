#include "dr_web_renderer.h"

namespace driderSDK {
  void WebRenderer::Init(GraphicsAPI* api, Quad* quad)
  {
     m_api = api;
     m_quad = quad;
     m_running = false;


     CefWindowInfo window_info;
     CefBrowserSettings browserSettings;
     browserSettings.windowless_frame_rate = 30;
     
     window_info.SetAsWindowless((HWND)m_api->m_hwnd);
     window_info.x = 0;
     window_info.y = 0;
     window_info.width = quad->width;
     window_info.height = quad->height;
     renderHandler = new RenderHandler(quad->width, quad->height);
     renderHandler->init(m_api,m_quad);
     browserClient = new BrowserClient(renderHandler);

     browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://www.google.com", browserSettings, nullptr);
     startRendering();

     CefRefPtr<CefFrame> frame = browser->GetMainFrame();
     frame->ExecuteJavaScript("alert('Drider Browser xdXDxDxDxDX');", frame->GetURL(), 0);

  }
  void WebRenderer::Destroy()
  {
    browser = nullptr;
    browserClient = nullptr;
    renderHandler = nullptr;
    CefShutdown();
  }
  void WebRenderer::captureInput()
  {
    CefMouseEvent mouseEvent;
    mouseEvent.x = 20;
    mouseEvent.y = 20;
    browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
    //browser->GetHost()->SendKeyEvent(...);
    // browser->GetHost()->SendMouseMoveEvent(...);
    // browser->GetHost()->SendMouseClickEvent(...);
    // browser->GetHost()->SendMouseWheelEvent(...);
  }
  void WebRenderer::startRendering()
  {
    m_running = true;
    //CefRunMessageLoop();
  }
  void WebRenderer::stoptRendering()
  {
    if (m_running) {
      m_running = false;
      //CefQuitMessageLoop();
    }
  }
  void WebRenderer::update()
  {
    CefDoMessageLoopWork();
  }
  void WebRenderer::loadURL(std::string path)
  {
    browser->GetMainFrame()->LoadURL(path);
  }
}