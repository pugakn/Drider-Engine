#include "dr_socket.h"
#include "dr_network_manager.h"

namespace driderSDK {

Socket::Socket() 
: m_blocking(true),
  m_handle(NetworkManager::getInvalidHandle()) 
{}

Socket::~Socket() {
  close();
}
void
Socket::setBlockMode(bool block) {
  if (isValid()) {
    NetworkManager::setSocketBlockMode(m_handle, block);
  }
}

SocketHandle 
Socket::getHandle() {
  return m_handle;
}

bool 
Socket::isValid() {
  return NetworkManager::isSocketValid(m_handle);
}
void 
Socket::close() {
  if (isValid()) {
    NetworkManager::closeSocket(m_handle);
    m_handle = NetworkManager::getInvalidHandle();
  }
}
}