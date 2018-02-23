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
     size_t windowHandle = 0;
     window_info.SetAsWindowless(nullptr);
     renderHandler = new RenderHandler(512,512);
     renderHandler->init(m_api,m_quad);
     browserClient = new BrowserClient(renderHandler);

     browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://deanm.github.io/pre3d/monster.html", browserSettings, nullptr);

     startRendering();

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
    CefMouseEvent event;
    // browser->GetHost()->SendKeyEvent(...);
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

    //CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    //frame->ExecuteJavaScript("alert('ExecuteJavaScript works!');", frame->GetURL(), 0);
    CefDoMessageLoopWork();
  }
  void WebRenderer::loadURL(std::string path)
  {
    browser->GetMainFrame()->LoadURL(path);
  }
}