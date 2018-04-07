#include "..\Include\dr_scene_editor.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_blend_state.h>
#include <dr_texture.h>
#include <dr_resource_manager.h>
#include <dr_aabb_collider.h>
#include <windows.h>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
namespace driderSDK {

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
        v.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + _T(".folder") + _T("']);"));
        read_directory(name + _T("\\") + data.cFileName, v);
      }
      else
      {
        auto ext = PathFindExtension(data.cFileName);
        v.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + TString(data.cFileName) + _T("','") + parent + _T("','") + ext + _T("']);"));
      }
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
}
void updateFolders(WebRenderer& webRenderer) {
  TString root(_T("Assets"));
  std::vector<TString> folders;
  folders.push_back(_T("webix.callEvent('WEBIX_AddFile',['") + root + _T("','") + root + _T("','") + _T(".folder") + _T("']);"));
  read_directory(root, folders);
  for (auto &it : folders) {
    std::string str(it.begin(), it.end());
    webRenderer.executeJSCode(str);
  }
}


void SceneEditor::init(Viewport v)
{
  m_viewport = v;
  DrTextureDesc backDesc;
  backDesc.width = 1024;
  backDesc.height = 1024;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  m_RT = dr_gfx_unique(GraphicsAPI::getDevice().createRenderTarget(backDesc,1));


  initCameras();
  initUI();
  initSceneGraph();
}
void SceneEditor::update()
{
  webRenderer.update();
}
void SceneEditor::draw()
{
  const float clearColor[4]{ 1,0,1,1 };
  m_RT->clear(GraphicsAPI::getDeviceContext(), clearColor);
  //m_RT->set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //Draw Scene

  //WString buff;
  //std::vector<byte> bBuff;
  //buff.resize(m_sceneWidth * m_sceneHeight * 4);
  //m_RT->getTexture(0).getMemoryBuffer(GraphicsAPI::getDeviceContext(),bBuff);
  //for (size_t i = 0; i < buff.size(); i++)
  //{
  //  buff[i] = bBuff[i];
  //}

  //////std::cout << std::string("C_UpdateEditor('") + buff + std::string("',") + std::to_string(m_sceneWidth) + std::string(",") + std::to_string(m_sceneHeight) + std::string(");") << std::endl;

  //webRenderer.executeJSCode(WString(_T("C_UpdateEditor('")) +
  //  buff + WString(_T("',")) +
  //  std::to_wstring(m_sceneWidth) +
  //  WString(_T(",")) +
  //  std::to_wstring(m_sceneHeight) +
  //  WString(_T(");")));



  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  //GraphicsDriver::API().clear();
  webRenderer.setTexture();
  GraphicsAPI::getBlendState(DR_BLEND_STATES::kAlphaBlend).set(GraphicsAPI::getDeviceContext());
  quad.draw();
  GraphicsAPI::getBlendState(DR_BLEND_STATES::kOpaque).set(GraphicsAPI::getDeviceContext());
  //GraphicsDriver::API().swapBuffers();




}
std::shared_ptr<GameObject> 
SceneEditor::addGameObject(std::shared_ptr<GameObject> parent, 
                           const TString & name, 
                           const Vector3D & pos)
{
  auto node = std::make_shared<GameObject>();
  parent->addChild(node);
  SceneGraph::addObject(node);
  node->setName(name);
  node->getTransform().setPosition(pos);
  node->setParent(parent);

  return node;
}
void SceneEditor::initCameras()
{
  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"),
    m_viewport);

  m_camera->createProyection(45.f, 20.f, 3000.f);
  m_camera->getTransform().setPosition({ 0.f, 300.0f, -400 });
  m_camera->setTarget({ 0.0f, 200.f, 1.0f });

  m_leftCam = std::make_shared<Camera>(_T("LEFT_CAM"),
    m_viewport);

  m_leftCam->createProyection(45.f, 0.1f, 10000.f);
  m_leftCam->getTransform().setPosition({ -4000.f, 0000.f, 1000.f });
  m_leftCam->setTarget({ 0.f, 0.f, 1000.f });

  m_upCam = std::make_shared<Camera>(_T("UP_CAM"),
    m_viewport);

  m_upCam->createProyection(45.f, 0.1f, 10000.f);
  m_upCam->getTransform().setPosition({ 0.f, 4000.f, 0.f });
  m_upCam->setTarget({ 1.f, 1.f, 200.f });

  m_activeCam = m_leftCam;
}
void SceneEditor::initUI()
{
  quad.init();
  webRenderer.Init(m_viewport.width, m_viewport.height, BROWSER_MODE::kHeadless);
  webRenderer.loadURL("file:///C:/Users/Ulises/Documents/GitHub/Drider-Engine/DriderUIUnitTest/WebixTest/ss.html");

  webRenderer.registerJS2CPPFunction(std::make_pair("webixReady", [&](const CefRefPtr<CefListValue>& arguments) {
    updateFolders(webRenderer);
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("canvasReady", [&](const CefRefPtr<CefListValue>& arguments) {
    webRenderer.executeJSCode(std::string("C_GetSceneViewSize();"));
  }));


  webRenderer.registerJS2CPPFunction(std::make_pair("JS_GetSceneViewSize", [&](const CefRefPtr<CefListValue>& arguments) {
    m_sceneWidth = arguments->GetInt(1);
    m_sceneHeight = arguments->GetInt(2);
    std::cout << m_sceneWidth << " , " << m_sceneHeight << std::endl;
    sceneResized();
  }));


  //Scene Graph UI
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeName", [&](const CefRefPtr<CefListValue>& arguments) {
    std::string prevName = arguments->GetString(1);
    std::string newName = arguments->GetString(2);
    //TODO: Search name 
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeParent", [&](const CefRefPtr<CefListValue>& arguments) {
    std::string name = arguments->GetString(1);
    std::string newParent = arguments->GetString(2);
    //TODO: Search name 
    //TODO: Search parent name 
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_AddSceneGraphNode", [&](const CefRefPtr<CefListValue>& arguments) {
    TString name = arguments->GetString(1);
    TString parent = arguments->GetString(2);
    //TODO: Search parent name 
    addGameObject(SceneGraph::getRoot(),
      name,
      { 0, 0, 0 })->getTransform().scale({ 1, 1, 1 });

    webRenderer.executeJSCode("JS_ClearSceneGraphTree();");
    UI_UpdateSceneGraph();
  }));
  webRenderer.registerJS2CPPFunction(std::make_pair("C_ChangeSceneGraphNodeSelection", [&](const CefRefPtr<CefListValue>& arguments) {
    std::string name = arguments->GetString(1);
    //TODO: Search name 

  }));
}
void SceneEditor::initSceneGraph()
{
  //n->getParent()->addChild(l);
  auto root = SceneGraph::getRoot();

  addGameObject(root,
    _T("Quad"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(root,
    _T("Quad2"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(root,
    _T("Quad3"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(SceneGraph::getRoot()->getChild(_T("Quad")),
    _T("Quad4_child"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });

  addGameObject(SceneGraph::getRoot()->getChild(_T("Quad"))->getChild(_T("Quad4_child")),
    _T("Quad5_child"),
    { 50, 300, 200 })->getTransform().scale({ 40, 40, 40 });
}
void SceneEditor::sceneResized()
{
  m_RT.release();
  DrTextureDesc backDesc;
  backDesc.width = m_sceneWidth;
  backDesc.height = m_sceneHeight;
  backDesc.pitch = backDesc.width * 4;
  backDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  backDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;
  m_RT = dr_gfx_unique(GraphicsAPI::getDevice().createRenderTarget(backDesc, 1));

  WString buff;
  buff.resize(m_sceneWidth * m_sceneHeight * 4);
  UInt8 val = 255;
  for (size_t i = 0; i < buff.size(); i++)
  {
    buff[i] = 255;
  }
  //std::cout << std::string("C_UpdateEditor('") + buff + std::string("',") + std::to_string(m_sceneWidth) + std::string(",") + std::to_string(m_sceneHeight) + std::string(");") << std::endl;
  webRenderer.executeJSCode(WString(_T("C_UpdateEditor('")) +
                            buff + WString(_T("',")) +
                            std::to_wstring(m_sceneWidth) + 
                            WString(_T(",")) +
                            std::to_wstring(m_sceneHeight) +
                            WString(_T(");")));
}

void SceneEditor::UI_UpdateSceneGraph()
{
  
    webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
      _T("ROOT_NODE_X") + TString(_T("','")) + _T("ROOT_NODE_X") +
      WString(_T("');")));
  std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> search = 
    [&](const std::vector<std::shared_ptr<GameObject>>& children) {
    for (auto &it : children) {
      auto name = it->getName();
      auto pName = it->getParent()->getName();

      webRenderer.executeJSCode(WString(_T("JS_AddSceneGraphNode('")) +
        name + TString(_T("','")) + pName + 
        WString(_T("');")));

      auto children2 = it->getChildren();
      search(children2);
    }
  };


  SceneGraph::getRoot()->getName();
  auto children = SceneGraph::getRoot()->getChildren();
  search(children);
}
}

