#pragma once

#include "dr_network_prerequisites.h"

#ifdef DR_PLATFORM_WINDOWS

#include <WinSock2.h>

#endif

namespace driderSDK {

#ifdef DR_PLATFORM_WINDOWS

using SocketHandle = SOCKET;

#endif

}
