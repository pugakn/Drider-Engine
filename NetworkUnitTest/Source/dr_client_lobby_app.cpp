#include "dr_client_lobby_app.h"

#include <functional>
#include <iostream>

#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_graphics_driver.h>
#include <dr_graphics_api.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_network_manager.h>
#include <dr_string_utils.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"

namespace driderSDK {

void 
ClientLobby::postInit() {

  NetworkManager::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
                          m_viewport.width, 
                          m_viewport.height,
                          m_hwnd);
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));

  m_err = false;

  Client::init();

  auto deviceObj = GraphicsDriver::API().getDevice().getAPIObject();
  auto deviceContextObj = GraphicsDriver::API().getDeviceContext().getAPIObject();

  auto d3dDev = static_cast<ID3D11Device*>(deviceObj);
  auto d3dDevCont = static_cast<ID3D11DeviceContext*>(deviceContextObj);

  m_currentDraw = &ClientLobby::drawMainMenu;

  m_nameStr = "Pimpolloppp";
  m_nameStr.resize(13);
  m_msg.resize(30);

  ImGui_ImplDX11_Init(m_hwnd, d3dDev, d3dDevCont);

  initCallbacks();

  m_netLobby.Init(1024, 720, BROWSER_MODE::kPopUp);
  m_netLobby.loadURL("file:///netLobby/NetLobby.html");
  m_netLobby.registerJS2CPPFunction(std::make_pair("C_RequestLobbies", [&](const CefRefPtr<CefListValue>& arguments) {
    std::cout << "Requesting lobbies..." << std::endl;
    requestLobbies();
  }));
  m_netLobby.registerJS2CPPFunction(std::make_pair("C_LobbyCLick", [&](const CefRefPtr<CefListValue>& arguments) {
    Int32 id = arguments->GetInt(1);
    std::cout << "Connecting..." << std::endl;
    requestConnection(m_lobbies[id].ip, m_lobbies[id].port);
  }));
  m_netLobby.registerJS2CPPFunction(std::make_pair("C_SendMsg", [&](const CefRefPtr<CefListValue>& arguments) {
    String msg = arguments->GetString(1);
    std::cout <<"You: "<< msg << std::endl;
    sendMessage(StringUtils::toWString(msg));
  }));
  m_netLobby.registerJS2CPPFunction(std::make_pair("C_SetUsername", [&](const CefRefPtr<CefListValue>& arguments) {
    String name = arguments->GetString(1);
    std::cout << "New name: " <<name << std::endl;
    m_userName = StringUtils::toWString(name);
    m_nameStr = name;
  }));
  m_netLobby.registerJS2CPPFunction(std::make_pair("C_Disconnect", [&](const CefRefPtr<CefListValue>& arguments) {
    requestDisconnection();
  }));
  
}

void 
ClientLobby::postUpdate() {
  WebRenderer::update();
  ImGui_ImplDX11_NewFrame();
  
  Client::update();

  InputManager::update();


  static bool notClosed = true;

  if (notClosed){
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)m_viewport.width, 
                                     (float)m_viewport.height));

    ImGuiWindowFlags flags = 0;
    
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoResize;
    
    //ImGui::ShowTestWindow(&notClosed);
    ImGui::Begin("", &notClosed, flags);
    (this->*m_currentDraw)();
    ImGui::End();
  }
}

void 
ClientLobby::postRender() {
  GraphicsDriver::API().clear();
  ImGui::Render();
  GraphicsDriver::API().swapBuffers();
}

void 
ClientLobby::postDestroy() {
  WebRenderer::shutDown();
  Client::quit();

  ImGui_ImplDX11_Shutdown();

  InputManager::shutDown();
  GraphicsDriver::shutDown();
  NetworkManager::shutDown();
}

void ClientLobby::onResize() {
  ImGui_ImplDX11_InvalidateDeviceObjects();
  GraphicsDriver::API().resizeBackBuffer(m_viewport.width, 
                                         m_viewport.height);
  ImGui_ImplDX11_CreateDeviceObjects(); 
  ImGui_ImplDX11_CreateDeviceObjects(); 
}

void 
ClientLobby::initCallbacks() {
  auto mouseButtonDown = 
  [](UInt32 b, bool down)
  { 
    auto& io = ImGui::GetIO(); io.MouseClicked[b] = down; 
  };

  auto mouseMove = 
  []()
  { auto& io = ImGui::GetIO(); 
    auto pos = Mouse::getPosition(); 
    io.MousePos.x = pos.x; io.MousePos.y = pos.y; };

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed, 
                           MOUSE_BUTTON::kLeft, 
                           std::bind(mouseButtonDown, 0, true));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased, 
                           MOUSE_BUTTON::kLeft, 
                           std::bind(mouseButtonDown, 0, false));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonPressed, 
                           MOUSE_BUTTON::kRight, 
                           std::bind(mouseButtonDown, 1, true));

  Mouse::addButtonCallback(MOUSE_INPUT_EVENT::kButtonReleased, 
                           MOUSE_BUTTON::kRight, 
                           std::bind(mouseButtonDown, 1, false));

  Mouse::addMovedCallback(mouseMove);

  auto charEntered = 
  [](KEY_CODE::E key)
  {
    auto& io = ImGui::GetIO();
    UInt16 character = static_cast<UInt16>(Keyboard::getAsChar(key));
    io.KeysDown[character] = true;
    io.AddInputCharacter(character);  
  };

  auto keyUp = 
  [](KEY_CODE::E key) 
  {
    auto& io = ImGui::GetIO();
    UInt16 character = static_cast<UInt16>(Keyboard::getAsChar(key));
    io.KeysDown[character] = false;
  };

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyPressed, charEntered);

  Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::kKeyReleased, keyUp);
}

void 
ClientLobby::onJoinAccepted() {
  m_currentDraw = &ClientLobby::drawInGame;
  m_messages.clear();
  m_msg.assign(30, '\0');
  //IMPORTANT
  auto temp = m_nameStr.substr(0, m_nameStr.find_first_of('\0'));
  m_userName = StringUtils::toWString(temp);

  m_netLobby.executeJSCode("JS_OnLobbyConnected();");
}

void 
ClientLobby::onJoinDenied() {
  m_err = true;
}

void 
ClientLobby::onConnectionLoss() {
  m_err = true;  
}

void 
ClientLobby::onChatMsgReceived(WString&& clientName, WString&& msg) {
  
  m_messages.emplace_back(StringUtils::toString(clientName),
                          StringUtils::toString(msg)); 

  //JS
  m_netLobby.executeJSCode("JS_AddMesage('" + StringUtils::toString(clientName) + "'" + ",'" + 
                           StringUtils::toString(msg) + "')");
}

void 
ClientLobby::onLobbiesListReceived(LobbiesList&& lobbies) {
  m_lobbies = std::move(lobbies);

  //JS
  m_netLobby.executeJSCode("JS_ClearLobbyItems();");
  Int32 index = 0;
  for (auto& lobby : m_lobbies) {
    String lobbyName = "Lobby ";
    lobbyName += ((index) + 48);
    lobbyName += '\0';
    m_netLobby.executeJSCode("JS_AddLobbyItem(" + std::to_string(index) + ",'"+ lobbyName  + "');");
    index++;
  }
}

void 
ClientLobby::drawMainMenu() {

  ImGui::Text("Name: ");
  ImGui::SameLine();
  //ImGui::InputText("##", const_cast<char*>(m_nameStr.data()), m_nameStr.size());

  if (ImGui::Button("Get Lobbies")) {
    //Call to receive current lobbies
    requestLobbies();
  }
  
  Int32 index = 0;
  for(auto& lobby : m_lobbies) {
    String lobbyName = "Lobby ";
    lobbyName += ((index++) + 48);
    lobbyName += '\0';
    if (ImGui::Button(lobbyName.c_str())) {
      ImGui::OpenPopup("Loading");
      m_currentDraw = &ClientLobby::drawLoading;
      requestConnection(lobby.ip, lobby.port);
      m_timer.init();
    }
  }
}

void 
ClientLobby::drawInGame() {
  
  if (m_err) {
    ImGui::OpenPopup("ErrorDisconn");
    m_currentDraw = &ClientLobby::drawPopupDisconn;
    return;
  }

  if(ImGui::Button("Disconnect")) {
    requestDisconnection();
    m_currentDraw = &ClientLobby::drawMainMenu;
  }

  ImGui::BeginChild("Messages", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f,300)/*{m_viewport.width * 0.8f, 400}*/, false, ImGuiWindowFlags_HorizontalScrollbar);
  for (auto& msg : m_messages) {

    String d = msg.first + ": " + msg.second;
    d += '\0';
    ImGui::Text(d.data());
  }

  String user = "Username " + m_nameStr + '\0';
  ImGui::Text(user.c_str());
  ImGui::SameLine();
  ImGui::Text("Enter your msg: ");
  ImGui::InputText("##", const_cast<char*>(m_msg.data()), m_msg.size());
  ImGui::SameLine();
  if (ImGui::Button("Send")) {
    auto temp = m_msg.substr(0, m_msg.find_first_of('\0'));
    if (!temp.empty()) {
      sendMessage(StringUtils::toWString(temp));
      m_msg.assign(30, '\0');
    }
  }

  ImGui::EndChild();
}

void ClientLobby::drawPopupErrConn() {

  drawMainMenu();

  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoTitleBar;
  
  ImGui::SetNextWindowSize({300, 70});
  ImGui::SetNextWindowPos({m_viewport.width * 0.5f - 150, 
                          m_viewport.height * 0.5f - 35});

  if (ImGui::BeginPopupModal("ErrorJoining", nullptr, flags)) {

    
    ImGui::TextColored({255, 100, 40, 1}, "Error while trying to join server!");
        
    ImGui::SetCursorPosX(110);
    ImGui::SetCursorPosY(40);

    if (ImGui::Button("Close", {80, 25})) {
      ImGui::CloseCurrentPopup();
      m_currentDraw = &ClientLobby::drawMainMenu;
      m_err = false;
      requestLobbies();
    }

    ImGui::EndPopup();
  }

}

void 
ClientLobby::drawPopupDisconn() {
  
  drawInGame();

  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoTitleBar;

  ImGui::SetNextWindowSize({300, 70});
  ImGui::SetNextWindowPos({m_viewport.width * 0.5f - 150, 
                          m_viewport.height * 0.5f - 35});

  
  if (ImGui::BeginPopupModal("ErrorDisconn", nullptr, flags)) {

    
    ImGui::TextColored({255, 100, 40, 1}, "Connection with server lost!");
        
    ImGui::SetCursorPosX(110);
    ImGui::SetCursorPosY(40);

    if (ImGui::Button("Close", {80, 25})) {
      ImGui::CloseCurrentPopup();
      m_currentDraw = &ClientLobby::drawMainMenu;
      m_err = false;
      requestLobbies();
    }

    ImGui::EndPopup();
  }
}

void
ClientLobby::drawLoading() {
  
  drawMainMenu();

  auto time = m_timer.getSeconds();
  
  time /= 4.f;

  ImGui::SetNextWindowSize({200, 45});
  ImGui::SetNextWindowPos({m_viewport.width * 0.5f - 100, 
                          m_viewport.height * 0.5f - 25});

  bool error = false;

  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoTitleBar;

  if (ImGui::BeginPopupModal("Loading", nullptr, flags)) {

    static Int32 points = 0;
    static Timer* timer = nullptr;

    if (!timer) {
      timer = new Timer;
      timer->init();
    }

    if (timer->getSeconds() >= 0.5f) {
      points = points % 4 + 1;
      timer->init();
    }
    
    String p = "Connecting";
    p.append(points, '.');
    p += '\0';

    ImGui::Text(p.data());

    ImGui::ProgressBar(time, {-1, 0,}, "\0");
    
    if (time >= 1.f || m_err) {
      ImGui::CloseCurrentPopup();
      error = true;
    }

    ImGui::EndPopup();
  } 

  if (error) {
    ImGui::OpenPopup("ErrorJoining");
    m_currentDraw = &ClientLobby::drawPopupErrConn;
  }
}

}