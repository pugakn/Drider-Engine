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

#include <windows.h>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace driderSDK {



//#include <sys/types.h>
//#include <dirent.h>
//
//  void read_directory(const std::string& name, stringvec& v)
//  {
//    DIR* dirp = opendir(name.c_str());
//    struct dirent * dp;
//    while ((dp = readdir(dirp)) != NULL) {
//      v.push_back(dp->d_name);
//    }
//    closedir(dirp);
//  }

void read_directory(const TString& name, std::vector<TString>& v)
{
  TString pattern(name);
  TString parent(name);
  pattern.append(_T("\\*"));
  WIN32_FIND_DATA data;
  HANDLE hFind;
  parent = PathFindFileName(name.c_str());
  hFind = FindFirstFile(pattern.c_str(), &data);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      if (data.cFileName[0] == '.') continue;
      if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        v.push_back(_T("webix.callEvent('addFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + _T(".folder") + _T("']);"));
        read_directory(name + _T("\\") + data.cFileName, v);
      }
      else 
      {
        auto ext = PathFindExtension(data.cFileName);
        v.push_back(_T("webix.callEvent('addFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + ext + _T("']);"));
      }
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
}
void updateFolders(WebRenderer& webRenderer) {
  TString root(_T("Assets"));
  std::vector<TString> folders;
  folders.push_back(_T("webix.callEvent('addFile',['") + root + _T("','") + root + _T("','") + _T(".folder") + _T("']);"));
  read_directory(root, folders);
  for (auto &it : folders) {
    std::string str(it.begin(), it.end());
    webRenderer.executeJSCode(str);
  }
}

TestApplication::TestApplication()
  : viewport{0,0,1024, 768}, webRenderer(){
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
  webRenderer.Init(m_viewport.width, m_viewport.height,BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/WebixTest/ss.html");

  webRenderer.registerJS2CPPFunction(std::make_pair("webixReady", [&](const CefRefPtr<CefListValue>& arguments) {
    updateFolders(webRenderer);
  }));
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
