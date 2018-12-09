#pragma once

#include <vector>

#include <dr_packet_handler.h>
#include <dr_connection_enums.h>
#include <dr_time.h>
#include <dr_message_type.h>

namespace driderSDK {

class Vector3D;

class Client : private PacketHandler {
public:

  struct Lobby
  {
    UInt32 ip;
    UInt16 port;
    UInt8 pad[2];
  };

  using LobbiesList = std::vector<Lobby>;

protected:

  void
  init(const TString& globalServer = _T("127.0.0.1"));

  void
  update();

  void
  quit();

  //Called when a previus join request to a server is accepted
  virtual void
  onJoinAccepted() = 0;

  //Called when a previus join request to a server is denied
  virtual void
  onJoinDenied() = 0;

  //Called when the current connection with a server is loss
  virtual void
  onConnectionLoss() = 0;

  //Called when we receive the active servers
  virtual void
  onLobbiesListReceived(LobbiesList&& lobbies) = 0;

  //Called when we receive the game state
  virtual void
  onGameStatusReceived(UInt8 num_players,
                       std::vector<Vector3D> positions) = 0;

  //Called whe we receive a game directive
  virtual void
  onInstantiatePlayer(bool isLocalPlayer,
                      const TString& name,
                      const Vector3D& pos,
                      const Vector3D& dir) = 0;

  void
  requestConnection(UInt32 ip, UInt16 port);

  void
  requestDisconnection();

  void
  requestLobbies();

  void
  sendFunction(Packet packet);

private:

  virtual void
  processPacket(MessageData& msg) override;

  void
  joinAccepted(MessageData& msg);

  void
  joinDenied(MessageData& msg);

  void
  lobbiesList(MessageData& msg);

  void
  requestActive(MessageData& msg);

  void
  receiveGameStatus(MessageData& msg);

  void
  receiveServerDirective(MessageData& msg);

  void
  checkConnectionStatus();

  void
  closeConnection();

  void
  sendPackets();

protected:

  WString m_userName;

private:

  using Command = decltype(&processPacket);
  using CommandList = std::vector<std::pair<REQUEST_ID::E, Command>>;

  struct ServerData
  {
    UInt32 ip;
    UInt32 port;
    Timer timeOut;
  };

  CommandList m_gameCommands;
  CommandList m_lobbyCommands;
  CommandList* m_commands;

  ServerData m_currentServer;
  ServerData m_globalServer;

  SharedSocket m_socket;

  //Disconnect if we haven't receive nothing from the server
  const float m_maxTimeOut = 10.f;
};

}