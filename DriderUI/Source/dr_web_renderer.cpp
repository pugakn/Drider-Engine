#include "dr_web_renderer.h"

namespace driderSDK {
  void WebRenderer::Init(GraphicsAPI* api, Quad* quad)
  {
     m_api = api;
     m_quad = quad;
     m_running = false;

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
     
     window_info.SetAsWindowless((HWND)m_api->m_hwnd);
     //window_info.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
     //window_info.SetAsPopup((HWND)m_api->m_hwnd, "NAMAE");
     window_info.x = 0;
     window_info.y = 0;
     window_info.width = quad->width;
     window_info.height = quad->height;
     renderHandler = new RenderHandler(quad->width, quad->height);
     renderHandler->init(m_api,m_quad);
     browserClient = new BrowserClient(renderHandler);

     browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://www.google.com", browserSettings, nullptr);//http://ich.deanmcnamee.com/pre3d/monster.html
     startRendering();

     CefRefPtr<CefFrame> frame = browser->GetMainFrame();
     frame->ExecuteJavaScript("alert('Drider Browser xdXDxDxDxDX');", frame->GetURL(), 0);


     //Register input
     InputManager::instance().getMouse()->setEventCallback(this);
     InputManager::instance().getKeyboard()->setEventCallback(this);
  }
  void WebRenderer::Destroy()
  {
    browser = nullptr;
    browserClient = nullptr;
    renderHandler = nullptr;
    CefShutdown();
  }
  void WebRenderer::startRendering()
  {
    m_running = true;
    //CefRunMessageLoop(); // UI on other process
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
    if (m_running) 
      CefDoMessageLoopWork(); //UI on the same process
  }
  void WebRenderer::loadURL(std::string path)
  {
    browser->GetMainFrame()->LoadURL(path);
  }
}