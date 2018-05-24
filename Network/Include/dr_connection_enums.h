#pragma once

#include "dr_network_prerequisites.h"

namespace driderSDK {

struct DR_NETWORK_EXPORT Ports
{
  static const UInt16 LOBBY_SERVER;
  static const UInt16 GAME_SERVER;
};

namespace REQUEST_ID {
enum E : UInt32
{
  kServerCreated = 256, //Send by the game server when its created
  kClientConnect, //Send by the client to notify the server a connection
  kConnectToGame, 
  kReportStatus,
  kServerAccepted,
  kLobbiesList, //Send by the lobby server to the client
  kLobbiesListRequest,
  kLobbyIDRequest, //Send by the game server to the lobby server
  kLobbyID, //Send by the lobby server to the game server
  kActive //Send to notify lobby server that a game server still alive
};
}

namespace VER_NUM {
enum E : UInt32
{
  kN = 0x07070808 //Magic number
};
}

}
