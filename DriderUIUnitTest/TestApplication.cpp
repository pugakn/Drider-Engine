#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_time.h>
#include <dr_input_manager.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_texture.h>
namespace driderSDK {

TestApplication::TestApplication()
  : viewport{0,0,1280, 720}, webRenderer(){
}

TestApplication::~TestApplication() {
}
void
TestApplication::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
    m_viewport.width,
    m_viewport.height,
    m_hwnd);
  InputManager::startUp((SizeT)m_hwnd);
  Time::startUp();

  quad.init();
  webRenderer.Init(1280,720,BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/bulma/ss.html");//
  JSCallLambda func = [](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
    retval = CefV8Value::CreateString("MyRetVal");
    //exit(666);
  };
  webRenderer.registerJS2CPPFunction(std::make_pair("myfunc", func));
  webRenderer.executeJSCode("alert(myfunc())");

  int num = 10;
  webRenderer.registerJS2CPPFunction(std::make_pair("setNum",  [&num](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
    num = arguments[0]->GetIntValue();
    retval = CefV8Value::CreateInt(num);
  }));
  webRenderer.executeJSCode("alert(setNum(55))");

  webRenderer.registerJS2CPPFunction(std::make_pair("onload", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
    webRenderer.executeJSCode("changeTitle('hola')");
    webRenderer.executeJSCode("alert(setNum(55))");
  }));
}

void
TestApplication::postUpdate() {
  InputManager::capture();
  webRenderer.update();
}

void
TestApplication::postRender() {
  GraphicsDriver::API().clear();
  webRenderer.setTexture();
  GraphicsAPI::getBlendState(DR_BLEND_STATES::kAlphaBlend).set(GraphicsAPI::getDeviceContext());
  quad.draw();
  GraphicsAPI::getBlendState(DR_BLEND_STATES::kOpaque).set(GraphicsAPI::getDeviceContext());
  GraphicsDriver::API().swapBuffers();
}
void
TestApplication::postDestroy() {

}



}
