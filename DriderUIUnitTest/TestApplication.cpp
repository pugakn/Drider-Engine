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
  quad.init();
  webRenderer.Init(1280,720,BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/ExampleHTML/example.html");
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
  //auto ddd = webRenderer.getJSGlobalVar("testVal01");
}

void
TestApplication::postUpdate() {
  webRenderer.executeJSCode("window.myfunc()");
  webRenderer.update();
}

void
TestApplication::postRender() {
  GraphicsDriver::API().clear();
  webRenderer.setTexture();
  GraphicsAPI::getBlendStateAlphaB().set(GraphicsAPI::getDeviceContext());
  quad.draw();
  GraphicsAPI::getBlendStateOpaque().set(GraphicsAPI::getDeviceContext());
  GraphicsDriver::API().swapBuffers();
}
void
TestApplication::postDestroy() {

}



}
