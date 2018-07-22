#include <dr_messenger.h>
#include <typeinfo>

namespace driderSDK {

Messenger::Messenger() {

}

Messenger::~Messenger() {

}


void
Messenger::onStartUp() {

}

void
Messenger::onShutDown() {

}

std::vector<Packet>
Messenger::getMessages() {
  return Messenger::instance().m_packets;
}

void
Messenger::clearMessages() {
  Messenger::instance().m_packets.clear();
}

}