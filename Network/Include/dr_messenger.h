#pragma once
#include "dr_network_prerequisites.h"

#include <dr_module.h>
#include <vector>
#include <cstdarg>

#include "dr_packet.h"
#include "dr_message_type.h"
#include "dr_connection_enums.h"

namespace driderSDK {

class DR_NETWORK_EXPORT Messenger : public Module<Messenger> {

public:
  Messenger();

  ~Messenger();

  template<typename t0> static void
  sendFunction(UInt32 objID,
               FUNCTION_TYPE::E function,
               t0 var0) {

    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kExecuteFunction;

    pack << function;
    pack << objID;
    pack << var0;

    Messenger::instance().m_packets.emplace_back(pack);

  }

  template<typename t0, typename t1> static void
    sendFunction(UInt32 objID,
                 FUNCTION_TYPE::E function,
                 t0 var0,
                 t1 var1) {

    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kExecuteFunction;

    pack << function;
    pack << objID;
    pack << var0;
    pack << var1;

    Messenger::instance().m_packets.emplace_back(pack);

  }

  template<typename t0, typename t1, typename t2> static void
    sendFunction(UInt32 objID,
                 FUNCTION_TYPE::E function,
                 t0 var0,
                 t1 var1,
                 t2 var2) {

    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kExecuteFunction;

    pack << function;
    pack << objID;
    pack << var0;
    pack << var1;
    pack << var2;

    Messenger::instance().m_packets.emplace_back(pack);

  }

  static std::vector<Packet>
  getMessages();

  static void
  clearMessages();

private:
  void
  onStartUp();

  void
  onShutDown();

private:
  std::vector<Packet> m_packets; 

};

}