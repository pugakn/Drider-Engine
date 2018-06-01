#pragma once

#include "dr_network_prerequisites.h"

#if (DR_PLATFORM == DR_PLATFORM_WINDOWS)
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h> 
#include <arpa/inet.h> /* Needed for inet_ntop and inet_pton*/
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif

namespace driderSDK {

#if (DR_PLATFORM == DR_PLATFORM_WINDOWS)

using SocketHandle = SOCKET;

#else

using SocketHandle = Int32;

#endif

}
