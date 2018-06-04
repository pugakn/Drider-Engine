#pragma once

#include "dr_network_prerequisites.h"

namespace driderSDK {

struct Ports
{
  constexpr static UInt16 LOBBY_SERVER = 60300;
  constexpr static UInt16 GAME_SERVER = 60400;
};

namespace REQUEST_ID {
enum E : UInt32
{
  kLobbiesRequest = 50,
  kLobbiesList,
  kClientJoin,
  kClientLeave,
  kServerAccepted,
  kPortRequest,
  kCloseServer,
  kChatMsg,
  kJoinAccepted,
  kJoinDenied,
  kConnectionLoss,
  kNotifyActive,
  kRequestActive,
  kServerFull,
  kServerNotFull
};
}

namespace VER_NUM {
enum E : UInt32
{
  kN = 0x07070808 //Magic number
};
}

}
