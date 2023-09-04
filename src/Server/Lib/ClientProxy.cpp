#include "ClientProxy.h"

#include "Clock.h"

ClientProxy::ClientProxy(const SocketAddress& inSocketAddress, const std::string& inName, int inPlayerId)
    : deliveryNotificationManager(false, true) {
  playerId = inPlayerId;
  socketAddress = inSocketAddress;
  name = inName;
  lastPacketTime = Clock::Instance.GetTimeF();
}

void ClientProxy::UpdateLastPacketTime() {
  lastPacketTime = Clock::Instance.GetTimeF();
}
