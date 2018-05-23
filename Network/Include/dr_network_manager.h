#pragma once

#include <dr_module.h>

#include "dr_network_prerequisites.h"
#include "dr_socket.h"

namespace driderSDK {

class DR_NETWORK_EXPORT NetworkManager : public Module<NetworkManager>
{
 public:

   /**
   * Checks if a socket handle is in a valid state.
   *
   * @param handle
   *  Handle to the socket to test.
   * 
   * @return
   *   True if the socket is valid, false otherwise.
   */
  static bool 
  isSocketValid(SocketHandle handle);

  /**
  * Gets a socket handle representing an invalid socket.
  * 
  * @return
  *   Invalid representation of a socket.
  */
  static SocketHandle
  getInvalidHandle();

  /**
  * Creates a socket of a specific type.
  * 
  * @param type
  *  The type of the socket to be created (udp, tcp, etc.).
  * 
  * @return
  *   A valid handle to a new socket, and invalid handle if any error
  *   occurs while creating the socket.
  */
  static SocketHandle
  createSocket(SOCKET_TYPE::E type);

  static sockaddr_in
  getAddress(UInt32 address, UInt16 port);
  
  static sockaddr_in
  getAddress(const TString& address, UInt16 port);
  
  static UInt32 
  ipAddrStrToUInt(const TString& address);

  static TString
  ipAddrUIntToStr(UInt32 address);

  /**
  * Extracts the ip addres and port from the given address struct.
  */
  static void
  getAddrPort(sockaddr_in address, TString& outAddress, UInt16& outPort);

  static void
  getAddrPort(sockaddr_in address, UInt32& outAddress, UInt16& outPort);
  
  /**
  * Gets the error string from the last network operation.
  */
  static TString
  getNetworkErrorStr();

  /**
  * Gets the error code from the last network operation.
  */
  static Int32
  getNetworkError();

  static SOCKET_ERR::E
  getSocketError();

  /**
  * Sets the block mode to the specified socket.
  *
  * @param socket
  *  A handle to the socket.
  *
  * @param block
  *  Specifes wheter it should or shouldn't block.
  */
  static void
  setSocketBlockMode(SocketHandle socket, bool block);

  /**
  * Closes the specifed socket handle.
  */
  static void
  closeSocket(SocketHandle socket);
 private:
  void
  onStartUp();

  void
  onShutDown();
};

}
