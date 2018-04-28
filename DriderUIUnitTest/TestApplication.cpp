#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_time.h>
#include <dr_input_manager.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_resource_manager.h>
#include <dr_camera_manager.h>
#include <dr_renderman.h>
namespace driderSDK {
TestApplication::TestApplication()
{
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
  SceneGraph::startUp();
  Time::startUp();
  ResourceManager::startUp();

  CameraManager::startUp();


  m_editor.init(m_viewport);

  ////Manage
  //webRenderer.registerJS2CPPFunction(std::make_pair("createFolder", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("renameFile", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("deleteFile", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("moveFile", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("updateFolders", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //  updateFolders(webRenderer);
  //}));
  ////Open
  //webRenderer.registerJS2CPPFunction(std::make_pair("openImage", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("openScript", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));
  //webRenderer.registerJS2CPPFunction(std::make_pair("openSound", [&](CefRefPtr<CefV8Value>& retval, const CefV8ValueList& arguments) {
  //}));



}

void
TestApplication::postUpdate() {
  Time::update();
  SceneGraph::update();
  InputManager::update();
  m_editor.update();
}

void
TestApplication::postRender() {
  GraphicsDriver::API().clear();
  m_editor.draw();
  GraphicsDriver::API().swapBuffers();
}
void
TestApplication::postDestroy() {
  GraphicsDriver::shutDown();
  ResourceManager::shutDown();
  SceneGraph::shutDown();
  InputManager::shutDown();
  Time::shutDown();
  Logger::shutDown();
}



}
